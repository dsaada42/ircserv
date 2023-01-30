/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dylan <dylan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:11:15 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/30 11:10:33 by dylan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

//===========================================================================================
//=                                                                                         =
//=                  CONSTRUCTOR / DESTRUCTOR / PUBLIC FUNCTIONS                            =
//=                                                                                         =
//===========================================================================================

// ----- Constructor / Destructor ------
irc::server::server(int port, str password) : 
    _port(port), _pass(password), _sock(AF_INET, SOCK_STREAM, TCP, port, INADDR_ANY, BACKLOG){
        init_cmd_map();
        _server_name = SERVER_NAME;
        _creation_date = ft_current_time();
    }

irc::server::~server( void ){
    delete_all_channels();
    delete_all_users();
    delete_all_received();
    delete_all_messages();
}

// ----- Getters -----
const int           & irc::server::port( void ) const           { return(_port); }
const int           & irc::server::sock_fd( void ) const        { return(_sock.fd()); }

// ----- Main loop -----
int         irc::server::run( void ){

    while( true ){
        update_sets();
        
        if (select(FD_SETSIZE, &read_sockets, &write_sockets, &except_sockets, NULL) < 0 ) {
            perror("select returned an error");
            throw(exitException());
        }

        manual_entry();
        
        handle_read_set();

        interprete_and_reply();
        
        handle_write_set();//gerer les messages perdus

        handle_users_timeout();
    }
}

//===========================================================================================
//=                                                                                         =
//=                CONNECTION ACCEPT / IDENTITY CHECKING / TIMEOUT                          =
//=                                                                                         =
//===========================================================================================

// ----- Connection Accept -----
void        irc::server::accept_connection( void ){
    int newfd;

    newfd = accept(sock_fd(), (struct sockaddr *)NULL, NULL );
    if (newfd < 0){
        std::cerr << "Error accepting connection" << std::endl;
        throw(exitException());
    }
    irc::user * new_user = new irc::user(newfd, get_time_ms());
    _users.insert(std::make_pair(newfd, new_user));
}

// ----- Timestamp -----
unsigned long   irc::server::get_time_ms( void ){
    struct timeval  time_now;
    gettimeofday(&time_now, NULL);
    return ((time_now.tv_sec * 1000) + (time_now.tv_usec / 1000));
}

// ----- Timeout Handler -----
void       irc::server::handle_users_timeout(void){
    std::map<int, irc::user*>::iterator it = _users.begin();
    irc::user                           *current;
    unsigned long                       inactive_time = 0;
    irc::message                        *msg;
    
    while (it != _users.end()){
        current = it->second;
        it++;
        inactive_time = get_time_ms() - current->timestamp();
        if (current->connected()){
            if (inactive_time > DISCONNECT_TRIGGER_TIME){
                std::cout << "Closing connection with user inactive for too long" << std::endl;
                delete_user(current);
            }
            else if (inactive_time > PING_TRIGGER_TIME && !current->ping()){
                std::cout << "Sending ping to inactive user" << std::endl;
                msg = cmd::cmd_ping( current->nickname() , current->fd() );
                _messages.push(msg);
                current->set_ping(true);
            }
        }
        else if (inactive_time > CONNECTION_TIMEOUT){
            std::cout << "Deleting user: connection procedure timeout" << std::endl;
            current->print();
            delete_user(current);
        }
    }
}

void        irc::server::say_welcome(irc::user *new_user){
    irc::message            *msg;

    msg = rpl::rpl_welcome(new_user->nickname(), new_user->fd());
    _messages.push(msg);
    msg = rpl::rpl_yourhost(new_user->nickname(), _server_name, SERVER_VERSION , new_user->fd());
    _messages.push(msg);
    msg = rpl::rpl_created(new_user->nickname(), _creation_date, new_user->fd()); // remplacer le champ vide par la date
    _messages.push(msg);
    msg = rpl::rpl_myinfo(new_user->nickname(), _server_name, SERVER_VERSION, "", "", new_user->fd()); // remplacer les champs vides par les modes user disponibles et mode channels disponibles
    _messages.push(msg);
    msg = rpl::rpl_bounce(new_user->nickname(), _server_name, SSTR(_port), new_user->fd());
    _messages.push(msg);
}
// ----- Identity checker -----
int         irc::server::handle_user_connection(irc::user *current){
    irc::message            *msg;
    
    while ((msg = current->extract_message("\r\n")) != NULL){
        //message received doesn't have the right format
        if (msg->parse_message() == FAILURE)
            return (FAILURE);
        if (msg->get_cmd() == "CAP"){
            ft_cap(msg);
        }
        else if (msg->get_cmd() == "PASS"){
            ft_pass(msg);
        }   
        else if (msg->get_cmd() == "NICK"){
            ft_nick(msg);
        }
        else if (msg->get_cmd() == "USER"){
            ft_user(msg);
        }
        else
            return (FAILURE); //message received is not part of connection process
    }
    //if everything ok 
    if (current->pass() && current->nickname().size() > 0 && current->fullname().size() > 0 ){
        current->set_connected(true);
        say_welcome(current);
    }
    return (SUCCESS);
}

//===========================================================================================
//=                                                                                         =
//=                                 READ / REPLY                                            =
//=                                                                                         =
//===========================================================================================

// ----- Read Handler -----
void        irc::server::handle_read_set( void ){
    std::map<int, irc::user*>::iterator it;
    irc::user                           *current;
    irc::message                        *msg;
    
    if (FD_ISSET(sock_fd(), &read_sockets)){
        std::cout << "New connection incoming" << std::endl;
        accept_connection();
    }
    it = _users.begin();
    while ( it != _users.end() ){
        current = (*it).second;
        it++; 
        if (FD_ISSET(current->fd(), &read_sockets)){
            //case reading error
            if (current->read_connection() == FAILURE){ 
                std::cout << "deleting user because connection lost" << std::endl;   
                delete_user(current);
            }
            else{
                //case user already fully connected
                if (current->connected()){
                    std::cout << "Received message from established connection" << std::endl;
                    while ((msg = current->extract_message("\r\n")) != NULL){
                        _received.push(msg);
                    }    
                }
                //case user still connecting
                else{
                    handle_user_connection(current);
                }                
            }
        }
        //case exception occurred on connection
        else if (FD_ISSET(current->fd(), &except_sockets)){
            std::cout << "Exception occured on connexion with user, closing connexion" << std::endl;
            delete_user(current);
        }
    }
}

// ----- Write Handler -----
void        irc::server::handle_write_set(void){
    message                 *current;
    std::queue<message*>    tmp_messages;

    while (_messages.size() > 0){        
        current = _messages.front();
        _messages.pop();
        if (FD_ISSET(current->get_fd(), &write_sockets)){
            if (current->send() == SUCCESS)
                delete current;
            else
                std::cerr << "failed sending message" << std::endl; // behaviour?
        }
        else{
            if (!find_user_by_fd(current->get_fd())) // si le message est destine a un fd inconnu 
                delete current;
            tmp_messages.push(current);
        }
    }
    _messages = tmp_messages;
}

// ----- Select helper -----
void       irc::server::update_sets( void )       {
    std::map<int, irc::user*>::iterator it = _users.begin();

    FD_ZERO(&read_sockets);
    FD_ZERO(&write_sockets);
    FD_ZERO(&except_sockets);
    FD_SET(sock_fd(), &read_sockets);
    FD_SET(sock_fd(), &write_sockets);
    FD_SET(sock_fd(), &except_sockets);
    for (; it != _users.end(); it++){
        FD_SET((*it).second->fd(), &read_sockets);
        FD_SET((*it).second->fd(), &write_sockets);
        FD_SET((*it).second->fd(), &except_sockets);
    }
    FD_SET(0, &read_sockets);
    FD_SET(1, &write_sockets);
}

//===========================================================================================
//=                                                                                         =
//=                         INIT FUNCTIONS AND MEMORY MANAGEMENT                            =
//=                                                                                         =
//===========================================================================================

// ----- Memory Handling -----
void        irc::server::delete_user(user *el){
    _users.erase(el->fd());
    delete el;
}
void        irc::server::delete_all_users( void ){
    std::map<int, user*>::iterator it = _users.begin();
    for (; it != _users.end(); it++){
        delete it->second;
    }
    _users.clear();
}
void        irc::server::delete_all_channels( void ){
    std::map<str, channel*>::iterator it = _channels.begin();
    for (; it != _channels.end(); it++){
        delete it->second;
    }
    _channels.clear();
}
void        irc::server::delete_all_messages(void){
    while( _messages.size() != 0){
        delete _messages.front();
        _messages.pop();
    }
}
void        irc::server::delete_all_received( void ){
    while( _received.size() != 0){
        delete _received.front();
        _received.pop();
    }
}

// ----- Init cmd map -----
void       irc::server::init_cmd_map(){
    _cmds.insert(std::make_pair("admin", &irc::server::ft_admin));                                      //OK
    _cmds.insert(std::make_pair("CAP", &irc::server::ft_cap));
    _cmds.insert(std::make_pair("INFO", &irc::server::ft_info));
    _cmds.insert(std::make_pair("INVITE", &irc::server::ft_invite));
    _cmds.insert(std::make_pair("JOIN", &irc::server::ft_join));
    _cmds.insert(std::make_pair("KICK", &irc::server::ft_kick));
    _cmds.insert(std::make_pair("KILL", &irc::server::ft_kill));
    _cmds.insert(std::make_pair("LIST", &irc::server::ft_list));
    _cmds.insert(std::make_pair("MODE", &irc::server::ft_mode));
    _cmds.insert(std::make_pair("NAMES", &irc::server::ft_names));
    _cmds.insert(std::make_pair("NICK", &irc::server::ft_nick));                                        //OK
    _cmds.insert(std::make_pair("NOTICE", &irc::server::ft_notice));                                    //OK
    _cmds.insert(std::make_pair("OPER", &irc::server::ft_oper));                                        //PRESQUE
    _cmds.insert(std::make_pair("PASS", &irc::server::ft_pass));                                        //OK
    _cmds.insert(std::make_pair("PART", &irc::server::ft_part));
    _cmds.insert(std::make_pair("PING", &irc::server::ft_ping));
    _cmds.insert(std::make_pair("PONG", &irc::server::ft_pong));                                        //OK
    _cmds.insert(std::make_pair("PRIVMSG", &irc::server::ft_privmsg));
    _cmds.insert(std::make_pair("QUIT", &irc::server::ft_quit));                                        //PRESQUE
    _cmds.insert(std::make_pair("STATS", &irc::server::ft_stats));
    _cmds.insert(std::make_pair("time", &irc::server::ft_time));                                        //OK
    _cmds.insert(std::make_pair("TOPIC", &irc::server::ft_topic));
    _cmds.insert(std::make_pair("USER", &irc::server::ft_user));                                        //OK
    _cmds.insert(std::make_pair("version", &irc::server::ft_version));                                  //OK
    _cmds.insert(std::make_pair("WHO", &irc::server::ft_who));
    _cmds.insert(std::make_pair("WHOIS", &irc::server::ft_whois));
    _cmds.insert(std::make_pair("WHOWAS", &irc::server::ft_whowas));
    _cmds.insert(std::make_pair("SUMMON", &irc::server::ft_summon));                                    //OK
    _cmds.insert(std::make_pair("USERS", &irc::server::ft_users));                                      //OK

}

//===========================================================================================
//=                                                                                         =
//=                         IRC LOGIC / RECEIVED COMMANDS                                   =
//=                                                                                         =
//===========================================================================================

// ----- Find User by -----
irc::user   *irc::server::find_user_by_fd(const int & fd){
    std::map<int, irc::user*>::iterator it;
    
    it = _users.find(fd);
    if (it != _users.end())
        return (it->second);
    return (NULL);
}

irc::user   *irc::server::find_user_by_nick(const str & nick){
    std::map<int, irc::user*>::iterator it;

    for (it = _users.begin(); it != _users.end(); it++){
        if (it->second->nickname() == nick)
            return (it->second);
    }
    return (NULL);
}

irc::channel    *irc::server::find_channel_by_name(const str & name){
    std::map<str, irc::channel*>::iterator it;

    it = _channels.find(name);
    if (it != _channels.end())
        return (it->second);
    return (NULL);    
}

// ----- Is ? ------
bool        irc::server::check_nickname_rules(const str & nick){
    str::size_type i = 0;
    
    if (nick.size() == 0 || nick.size() > 9)
        return (false);
    while (i < nick.size()){
        if (isalnum(nick.at(i)) || i == '-' || i == '[' || i == ']' || i == '\\' || i == '`' || i == '^' || i == '{' || i == '}')
            i++;
        else
            return (false);
    }
    return (true);
}
bool        irc::server::check_channel_rules(const str & chan){
    if (chan.size() > 200 || chan.size() < 2)
        return (false);
    if (chan.find(',') != str::npos || chan.find(0x7) != str::npos)
        return (false);
    if (chan.at(0) != '#' && chan.at(0) != '&')
        return (false);
    return (true);
}
bool        irc::server::is_a_nickname(const str & nick){
    if (!check_nickname_rules(nick))
        return (false);
    if (!find_user_by_nick(nick))
        return (false);
    return (true);
}
bool        irc::server::is_a_channel(const str & chan){
    if (!check_channel_rules(chan))
        return (false);
    if (!find_channel_by_name(chan))
        return (false);
    return(true);    
}

// ----- Message Interpreter + Reply generator -----
void        irc::server::interprete_and_reply( void ){
    std::map<int, irc::user*>::iterator it;
    irc::message                        *msg;
    void (irc::server::*funcp)(irc::message *);
    std::map<str, void (irc::server::*)(irc::message *)>::iterator itmap;

    while (_received.size() != 0 ){
        msg = _received.front();
        if (msg->parse_message() == SUCCESS){
            itmap = _cmds.find( msg->get_cmd() );
            if (itmap != _cmds.end()){
                funcp = itmap->second;
                (*this.*funcp)(msg);
            }
            else // en cas de commande non geree par le serveur
                _messages.push(err::err_unknowncommand(msg->get_cmd(), msg->get_fd()));
        }
        delete msg;
        _received.pop();
    }
}

// ================> ALL COMMANDS HANDLED BY SERVER <===================

void irc::server::ft_admin( irc::message * msg ){
    if (msg->get_params().empty())
        _messages.push(err::err_noadmininfo(msg->get_fd()));
    else if (msg->get_params() != SERVER_NAME)
        _messages.push(err::err_nosuchserver(msg->get_params(), msg->get_fd()));
    else
        _messages.push(err::err_noadmininfo(msg->get_fd()));
}
void irc::server::ft_cap( irc::message * msg ){(void)msg;}//ignore CAP messages
void irc::server::ft_info( irc::message * msg ){(void)msg;}
void irc::server::ft_invite( irc::message * msg ){(void)msg;} //-> invite a user to a channel
void irc::server::ft_join( irc::message * msg ){(void)msg;} //-> join a channel
void irc::server::ft_kick( irc::message * msg ){(void)msg;} //-> operator kicks user from channel
void irc::server::ft_kill( irc::message * msg ){(void)msg;} //-> operator kills user (delete user)
void irc::server::ft_list( irc::message * msg ){
    //lists all channels + topic if param is empty
    // if params not empty , lists the channels listed with their topics (separated by comas)
    // there is a space in the params, it must be the last param, and this param refers to a server
    (void)msg;
    //ERR_NOSUCHSERVER
    //RPL_LISTSTART
    //RPL_LIST
    //RPL_LISTEND
}
void irc::server::ft_mode(irc::message * msg){(void)msg;} // both channel and users
void irc::server::ft_names(irc::message * msg){
    //lists all nicknames visible on all visible channels (if a channel is private or secret, it won't appear unless the user is part of it)
    //params can be a list of channels (will get an answer only if channel is visible)
    //RPL_NAMREPLY
    //RPL_ENDOFNAMES 
    (void)msg;
}
void irc::server::ft_nick(irc::message * msg){
    irc::user       *current;
    
    if (msg->get_params().empty())
        _messages.push(err::err_nonicknamegiven(msg->get_fd()));
    else if (check_nickname_rules(msg->get_params()) == false)
        _messages.push(err::err_erroneusnickname(msg->get_params(), msg->get_fd()));
    else if (is_a_nickname(msg->get_params()) == false){
        current = find_user_by_fd(msg->get_fd());
        current->set_nickname(msg->get_params());
    }
    else
        _messages.push(err::err_nicknameinuse(msg->get_params(), msg->get_fd()));
}
void irc::server::ft_notice(irc::message * msg){(void)msg;}//(won't trigger any reply since we are a server)
void irc::server::ft_oper(irc::message * msg){
    std::vector<str>    args;
    irc::user           *current;

    args = ft_split(msg->get_params(), " ");
    if (args.size() < 2){
        std::cout << "need more params" << std::endl;
        _messages.push(err::err_needmoreparams(msg->get_cmd(), msg->get_fd()));
        return;
    }
    if ((current = find_user_by_nick(args[0])) != NULL){
        if (current->fd() == msg->get_fd()){
            if (args[1] == OPER_PASSWORD){
            _messages.push(rpl::rpl_youreoper(current->nickname(), msg->get_fd()));
            current->set_oper(true);
            //generate broadcast message MODE +o to inform other users of the new oper
            }
            else{
                _messages.push(err::err_passwdmissmatch(msg->get_fd()));
            }
        }
    }
    else{
        _messages.push(err::err_nooperhost(msg->get_fd()));
    }
}
void irc::server::ft_pass(irc::message * msg){
    irc::user * current;

    if (msg->get_params().empty())
        _messages.push(err::err_needmoreparams(msg->get_cmd(), msg->get_fd()));   
    else{
        current = find_user_by_fd(msg->get_fd());
        if (current->connected()){
            _messages.push(err::err_alreadyregistred(msg->get_fd()));
        }
        else{
            if (msg->get_params() == _pass)
                current->set_pass(true);
            else
                current->set_pass(false);
        }
    }
}
void irc::server::ft_part(irc::message * msg){
    // quits channel (with optionnal message sent to current users in channel)
    (void)msg;
}
void irc::server::ft_ping(irc::message * msg){(void)msg;}// server will ignore message but listen to the ping to refresh inactivity timestamp of sending user
void irc::server::ft_pong(irc::message * msg){
    if (msg->get_params().empty()){
        _messages.push(err::err_noorigin(msg->get_fd()));
    }
    else if (msg->get_params() != SERVER_NAME){
        _messages.push(err::err_nosuchserver(msg->get_params(), msg->get_fd()));
    }
    else //attention au cas ou user send QUIT then PONG --> need security on find_user
        find_user_by_fd(msg->get_fd())->set_ping(false);
}
void irc::server::ft_privmsg(irc::message * msg){(void)msg;}
void irc::server::ft_quit(irc::message * msg){
    irc::user   *current;

    // if (!msg->get_trailing().empty())
    //last message received from a user, if Quit is received , disconnect user and send message to all
    current = find_user_by_fd(msg->get_fd());
    delete_user(current);
    (void)msg;
}
void irc::server::ft_stats(irc::message * msg){(void)msg;}
void irc::server::ft_time(irc::message * msg){
    if (!msg->get_params().empty() && msg->get_params() != SERVER_NAME){
        _messages.push(err::err_nosuchserver(msg->get_params(), msg->get_fd()));
    }
    else
        _messages.push(rpl::rpl_time(SERVER_NAME, msg->get_fd()));
}
void irc::server::ft_topic(irc::message * msg){(void)msg;} //send topic of given channel 
void irc::server::ft_user(irc::message * msg){
    irc::user * current;
    std::vector<str> args;

    if (msg->get_params().empty() || msg->get_trailing().empty()){
        _messages.push(err::err_needmoreparams(msg->get_cmd(), msg->get_fd()));   
        return;
    }
    args = ft_split(msg->get_params(), " ");
    if (args.size() < 3){
        _messages.push(err::err_needmoreparams(msg->get_cmd(), msg->get_fd()));
        return;
    }
    current = find_user_by_fd(msg->get_fd());
    if (current->username().size() != 0){
        _messages.push(err::err_alreadyregistred(msg->get_fd()));
        return;
    }
    if (current){
        current->set_username(args[0]);
        current->set_fullname(msg->get_trailing());
    }
}

void irc::server::ft_version(irc::message * msg){
    irc::user *current;

    current = find_user_by_fd(msg->get_fd());
    if (!current)
        return;
    if (msg->get_params().empty() || msg->get_params() == SERVER_NAME)
        _messages.push(rpl::rpl_version(current->nickname(), SERVER_VERSION, "", SERVER_NAME , "The one and only", msg->get_fd()));
    else
        _messages.push(err::err_nosuchserver(msg->get_params(), msg->get_fd()));
}
void irc::server::ft_who(irc::message * msg){(void)msg;}
void irc::server::ft_whois(irc::message * msg){(void)msg;}
void irc::server::ft_whowas(irc::message * msg){(void)msg;}
//disabled commands
void irc::server::ft_users(irc::message * msg){ _messages.push(err::err_usersdisabled(msg->get_fd())); }
void irc::server::ft_summon(irc::message * msg){ _messages.push(err::err_summondisabled(msg->get_fd())); }

//===========================================================================================
//=                                                                                         =
//=                  TESTING FUNCTIONS -> NOT FOR FINAL VERSION                             =
//=                                                                                         =
//===========================================================================================

// ----- Debug / Print -----
void        irc::server::print_users( void ){
    std::map<int, irc::user*>::iterator it;
    
    for (it = _users.begin(); it != _users.end(); it++){
        it->second->print();
    }
}

// ----- Manual entry (stdin) handler -----
int         irc::server::manual_entry( void ){
    irc::message *msg;
    
    if (FD_ISSET(0, &read_sockets)){
        _admin.read_connection();
    }
    while ((msg = _admin.extract_message("\n")) != NULL){
        _received.push(msg);
        if (msg->get_message() == "exit")
            throw (exitException());
    }
    return (SUCCESS);
}