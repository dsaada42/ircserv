/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dylan <dylan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:11:15 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/26 13:10:30 by dylan            ###   ########.fr       */
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
void        irc::server::accept_connection( void )              {
    int newfd;

    newfd = accept(sock_fd(), (struct sockaddr *)NULL, NULL );
    if (newfd < 0){
        std::cerr << "Error accepting connection" << std::endl;
        throw(exitException());
    }
    irc::user * new_user = new irc::user(newfd, get_time_ms());
    new_user->set_connected(true);
    _users.insert(std::make_pair(newfd, new_user));
    irc::message reply(":shittyIRC 001 dsaada :Welcome to our shitty IRC server\r\n", newfd);
    reply.send();
    print_users();
}

// ----- Identity checker -----
void        irc::server::handle_user_connection(irc::user *current){
    (void)current;
    //gerer la procedure de connection de l'user
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
        if (inactive_time > DISCONNECT_TRIGGER_TIME){
            std::cout << "Closing connection with user inactive for too long" << std::endl;
            delete_user(current);
        }
        else if (inactive_time > PING_TRIGGER_TIME && !current->ping()){
            std::cout << "Sending ping to inactive user" << std::endl;
            msg = cmd::cmd_ping( "dsaada" , current->fd() );
            msg->create_message();
            _messages.push(msg);
            current->set_ping(true);
        }
    }
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
            if (current->connected()){    
                std::cout << "Received message from established connection" << std::endl;
                if (current->read_connection() == FAILURE){
                    std::cout << "deleting user because connection lost" << std::endl;   
                    delete_user(current);
                }
                else{
                    while ((msg = current->extract_message("\r\n")) != NULL){
                        _received.push(msg);
                    }
                }
            }
            else
                handle_user_connection(current);
        }
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
                std::cerr << "failed sending message" << std::endl;
        }
        else{
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
    _cmds.insert(std::make_pair("ADMIN", &irc::server::ft_admin));
    _cmds.insert(std::make_pair("CAP", &irc::server::ft_cap));
    _cmds.insert(std::make_pair("ERROR", &irc::server::ft_error));
    _cmds.insert(std::make_pair("INFO", &irc::server::ft_info));
    _cmds.insert(std::make_pair("INVITE", &irc::server::ft_invite));
    _cmds.insert(std::make_pair("JOIN", &irc::server::ft_join));
    _cmds.insert(std::make_pair("KICK", &irc::server::ft_kick));
    _cmds.insert(std::make_pair("KILL", &irc::server::ft_kill));
    _cmds.insert(std::make_pair("LIST", &irc::server::ft_list));
    _cmds.insert(std::make_pair("MODE", &irc::server::ft_mode));
    _cmds.insert(std::make_pair("NAMES", &irc::server::ft_names));
    _cmds.insert(std::make_pair("NICK", &irc::server::ft_nick));
    _cmds.insert(std::make_pair("NOTICE", &irc::server::ft_notice));
    _cmds.insert(std::make_pair("OPER", &irc::server::ft_oper));
    _cmds.insert(std::make_pair("PASS", &irc::server::ft_pass));
    _cmds.insert(std::make_pair("PART", &irc::server::ft_part));
    _cmds.insert(std::make_pair("PING", &irc::server::ft_ping));
    _cmds.insert(std::make_pair("PONG", &irc::server::ft_pong));
    _cmds.insert(std::make_pair("PRIVMSG", &irc::server::ft_privmsg));
    _cmds.insert(std::make_pair("QUIT", &irc::server::ft_quit));
    _cmds.insert(std::make_pair("STATS", &irc::server::ft_stats));
    _cmds.insert(std::make_pair("TIME", &irc::server::ft_time));
    _cmds.insert(std::make_pair("TOPIC", &irc::server::ft_topic));
    _cmds.insert(std::make_pair("USER", &irc::server::ft_user));
    _cmds.insert(std::make_pair("VERSION", &irc::server::ft_version));
    _cmds.insert(std::make_pair("WHO", &irc::server::ft_who));
    _cmds.insert(std::make_pair("WHOIS", &irc::server::ft_whois));
    _cmds.insert(std::make_pair("WHOWAS", &irc::server::ft_whowas));

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
        }
        delete msg;
        _received.pop();
    }
}

void irc::server::ft_admin( irc::message * msg ){(void)msg;}
void irc::server::ft_cap( irc::message * msg ){(void)msg;}
void irc::server::ft_error( irc::message * msg ){(void)msg;}
void irc::server::ft_info( irc::message * msg ){(void)msg;}
void irc::server::ft_invite( irc::message * msg ){(void)msg;}
void irc::server::ft_join( irc::message * msg ){(void)msg;}
void irc::server::ft_kick( irc::message * msg ){(void)msg;}
void irc::server::ft_kill( irc::message * msg ){(void)msg;}
void irc::server::ft_list( irc::message * msg ){(void)msg;}
void irc::server::ft_mode(irc::message * msg){(void)msg;}
void irc::server::ft_names(irc::message * msg){(void)msg;}
void irc::server::ft_nick(irc::message * msg){(void)msg;}
void irc::server::ft_notice(irc::message * msg){(void)msg;}
void irc::server::ft_oper(irc::message * msg){(void)msg;}
void irc::server::ft_pass(irc::message * msg){(void)msg;}
void irc::server::ft_part(irc::message * msg){(void)msg;}
void irc::server::ft_ping(irc::message * msg){(void)msg;}
void irc::server::ft_pong(irc::message * msg){
    if (msg->get_params().empty()){
        _messages.push(err::err_noorigin(msg->get_fd()));
    }
    else if (msg->get_params() != SERVER_NAME){
        _messages.push(err::err_nosuchserver(msg->get_params(), msg->get_fd()));
    }
    else
        find_user_by_fd(msg->get_fd())->set_ping(false);
}
void irc::server::ft_privmsg(irc::message * msg){(void)msg;}
void irc::server::ft_quit(irc::message * msg){(void)msg;}
void irc::server::ft_stats(irc::message * msg){(void)msg;}
void irc::server::ft_time(irc::message * msg){(void)msg;}
void irc::server::ft_topic(irc::message * msg){(void)msg;}
void irc::server::ft_user(irc::message * msg){(void)msg;}
void irc::server::ft_version(irc::message * msg){(void)msg;}
void irc::server::ft_who(irc::message * msg){(void)msg;}
void irc::server::ft_whois(irc::message * msg){(void)msg;}
void irc::server::ft_whowas(irc::message * msg){(void)msg;}



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

int        irc::server::send_message(const int & fd, irc::message msg){
    str result;

    result = msg.get_message();
    return (write(fd, result.c_str(), result.size()));
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