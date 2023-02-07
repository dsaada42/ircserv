/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:11:15 by dsaada            #+#    #+#             */
/*   Updated: 2023/02/07 12:21:38 by dsaada           ###   ########.fr       */
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
        _creation_time = get_time_ms();
        
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
        
        handle_write_set();

        handle_users_timeout();

        clear_useless();
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
                delete_user(current, "User inactive for too long");
            }
            else if (inactive_time > PING_TRIGGER_TIME && !current->ping()){
                msg = cmd::cmd_ping( current->nickname() , current->fd() );
                _messages.push(msg);
                current->set_ping(true);
            }
        }
        else if (inactive_time > CONNECTION_TIMEOUT){
            std::cout << "Deleting user: connection procedure timeout" << std::endl;
            current->print();
            delete_user(current, "Connection procedure timeout");
        }
    }
}

void        irc::server::say_welcome(irc::user *new_user){
    irc::message            *msg;

    msg = rpl::rpl_welcome(new_user->nickname(), new_user->fd());
    _messages.push(msg);
    msg = rpl::rpl_yourhost(new_user->nickname(), _server_name, SERVER_VERSION , new_user->fd());
    _messages.push(msg);
    msg = rpl::rpl_created(new_user->nickname(), _creation_date, new_user->fd());
    _messages.push(msg);
    msg = rpl::rpl_myinfo(new_user->nickname(), _server_name, SERVER_VERSION, UMODES, CMODES, new_user->fd()); // remplacer le champ vide par les modes channels disponibles
    _messages.push(msg);
    msg = rpl::rpl_bounce(new_user->nickname(), _server_name, SSTR(_port), new_user->fd());
    _messages.push(msg);
}
// ----- Identity checker -----
int         irc::server::handle_user_connection(irc::user *current){
    irc::message            *msg;
    
    while ((msg = current->extract_message("\r\n")) != NULL){
        if (msg->parse_message() == FAILURE){
            delete msg;
            return (FAILURE);
        }
        if (msg->get_cmd() == "CAP"){
            ft_cap(msg);
        }
        else if (msg->get_cmd() == "PASS"){
            ft_pass(msg);
        }   
        else if (msg->get_cmd() == "NICK"){
            if (current->pass())
                ft_nick(msg);
            else{
                delete_user(current, "Error in connection procedure");
                delete msg;
                return(FAILURE);   
            }
        }
        else if (msg->get_cmd() == "USER"){
            if (current->pass() && current->nickname().size() != 0)
                ft_user(msg);
            else{
                delete msg;
                delete_user(current, "Error in connection procedure");
                return(FAILURE);   
            }
        }
        else{
            delete msg;   
            return (FAILURE);
        }
        delete msg;
    }
    if (current->pass() && current->nickname().size() > 0 && current->fullname().size() > 0 ){
        current->set_connected(true);
        current->new_timestamp();
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
            if (current->read_connection() == FAILURE){ 
                delete_user(current, "Connection lost");
            }
            else{
                if (current->connected()){
                    while ((msg = current->extract_message("\r\n")) != NULL){
                        _received.push(msg);
                    }    
                }
                else
                    handle_user_connection(current);            
            }
        }
        //case exception occurred on connection
        else if (FD_ISSET(current->fd(), &except_sockets)){
            delete_user(current, "Connection error occurred");
        }
    }
}

// ----- Write Handler -----
void        irc::server::handle_write_set(void){
    message                 *current;
    irc::user               *usr;
    std::queue<message*>    tmp_messages;

    while (_messages.size() > 0){        
        current = _messages.front();
        _messages.pop();
        if (FD_ISSET(current->get_fd(), &write_sockets)){
            if (current->send() == SUCCESS){
                usr = find_user_by_fd(current->get_fd());
                usr->incr_sent_cmd();
                usr->incr_sent_bytes(current->get_message().size());
            }
            delete current;
        }
        else{
            if (!find_user_by_fd(current->get_fd()))
                delete current;
            else
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
void        irc::server::delete_user(user *el, str reason){
    std::map<str, channel*>::iterator itc;
    std::map<int, user*>::iterator itu;

    if (reason.size() != 0 && el->connected() == true){
        for (itu = _users.begin(); itu != _users.end(); itu++){
            if (itu->second->nickname() != el->nickname())
                _messages.push(cmd::cmd_quit(user_prefix(el), reason, itu->second->fd()));    
        }
    }
    for (itc = _channels.begin(); itc != _channels.end(); itc++){
        itc->second->delete_user(el);
    }
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
    _cmds.insert(std::make_pair("CAP", &irc::server::ft_cap));                                          //OK
    _cmds.insert(std::make_pair("info", &irc::server::ft_info));                                        //OK
    _cmds.insert(std::make_pair("INVITE", &irc::server::ft_invite));                                    //OK
    _cmds.insert(std::make_pair("JOIN", &irc::server::ft_join));                                        //OK
    _cmds.insert(std::make_pair("KICK", &irc::server::ft_kick));                                        //OK
    _cmds.insert(std::make_pair("kill", &irc::server::ft_kill));                                        //OK
    _cmds.insert(std::make_pair("LIST", &irc::server::ft_list));                                        //OK
    _cmds.insert(std::make_pair("MODE", &irc::server::ft_mode));                                        //OK 
    _cmds.insert(std::make_pair("NAMES", &irc::server::ft_names));                                      //OK
    _cmds.insert(std::make_pair("NICK", &irc::server::ft_nick));                                        //OK
    _cmds.insert(std::make_pair("NOTICE", &irc::server::ft_notice));                                    //OK
    _cmds.insert(std::make_pair("OPER", &irc::server::ft_oper));                                        //OK
    _cmds.insert(std::make_pair("PASS", &irc::server::ft_pass));                                        //OK
    _cmds.insert(std::make_pair("PART", &irc::server::ft_part));                                        //OK (gestion de role oper dans le channel directement?)
    _cmds.insert(std::make_pair("PING", &irc::server::ft_ping));                                        //OK
    _cmds.insert(std::make_pair("PONG", &irc::server::ft_pong));                                        //OK
    _cmds.insert(std::make_pair("PRIVMSG", &irc::server::ft_privmsg));                                  //OK
    _cmds.insert(std::make_pair("QUIT", &irc::server::ft_quit));                                        //OK
    _cmds.insert(std::make_pair("stats", &irc::server::ft_stats));                                      //OK
    _cmds.insert(std::make_pair("time", &irc::server::ft_time));                                        //OK
    _cmds.insert(std::make_pair("TOPIC", &irc::server::ft_topic));                                      //OK
    _cmds.insert(std::make_pair("USER", &irc::server::ft_user));                                        //OK
    _cmds.insert(std::make_pair("version", &irc::server::ft_version));                                  //OK
    _cmds.insert(std::make_pair("WHO", &irc::server::ft_who));                                          //EN COURS                               
    _cmds.insert(std::make_pair("WHOIS", &irc::server::ft_whois));                                      //EN COURS 
    _cmds.insert(std::make_pair("WHOWAS", &irc::server::ft_whowas));                                    //OK 
    _cmds.insert(std::make_pair("SUMMON", &irc::server::ft_summon));                                    //OK
    _cmds.insert(std::make_pair("USERS", &irc::server::ft_users));                                      //OK
}

//----- Clear useless -----
void        irc::server::clear_useless(void){
    std::map<str, irc::channel*>::iterator it;
    irc::channel *chan;
    
    it = _channels.begin();
    while (it != _channels.end()){
        chan = it->second;
        it++;
        if (chan->is_empty()){//delete useless 
            _channels.erase(chan->get_name());
            std::cout << "deleting empty channel: " << chan->get_name() << std::endl;
            delete chan;
        }
    }
}

//===========================================================================================
//=                                                                                         =
//=                     IRC LOGIC / (RECEIVED COMMANDS in separate file                     =
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

// ----- Is channel / Is user ------
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

str         irc::server::user_prefix(irc::user *usr){
    return (usr->nickname()+"!~"+usr->username()+"@"+_server_name);
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
            else
                _messages.push(err::err_unknowncommand(msg->get_cmd(), msg->get_fd()));
        }
        delete msg;
        _received.pop();
    }
}

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