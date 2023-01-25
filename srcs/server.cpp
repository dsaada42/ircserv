/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:11:15 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/25 12:47:38 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

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

// ----- Network -----
void        irc::server::accept_connection( void )              {
    int newfd;

    newfd = accept(sock_fd(), (struct sockaddr *)NULL, NULL );
    if (newfd < 0){
        std::cerr << "Error accepting connection" << std::endl;
        throw(exitException());
    }
    irc::user * new_user = new irc::user(newfd, get_time_ms());
    _users.insert(std::make_pair(newfd, new_user));
    irc::message reply(":my_server 001 dsaada :Welcome to our shitty IRC server\r\n");
    send_message(newfd, reply);
}

int        irc::server::send_message(const int & fd, irc::message msg){
    str result;

    result = msg.get_message();
    return (write(fd, result.c_str(), result.size()));
}

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
                    while ((msg = current->extract_message("\r\n")) != NULL)
                        _received.push(msg);  
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

void        irc::server::handle_user_connection(irc::user *current){
    (void)current;
    //gerer la procedure de connection de l'user
}

// ----- Write Handler -----
void        irc::server::handle_write_set(void){
    message                 *current;
    std::queue<message*>    tmp_messages;

    while (_messages.size() > 0){        
        current = _messages.front();
        _messages.pop();
        if (FD_ISSET(current->to(), &write_sockets)){
            if (current->send() == SUCCESS)
                delete current;            
            else
                tmp_messages.push(current);
        }
        else
            tmp_messages.push(current);
    }
    _messages = tmp_messages;
}

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
    }
}

// ----- Timeout / load handler -----
unsigned long   irc::server::get_time_ms( void ){
    struct timeval  time_now;
    gettimeofday(&time_now, NULL);
    return ((time_now.tv_sec * 1000) + (time_now.tv_usec / 1000));
}
void       irc::server::handle_users_timeout(void){
    std::map<int, irc::user*>::iterator it = _users.begin();
    irc::user                           *current;
    unsigned long                       inactive_time = 0;
    
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
            //send ping
            current->set_ping(true);
        }
    }
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

// ----- Message Interpreter + Reply generator -----
void        irc::server::interprete_and_reply( void ){
    std::map<int, irc::user*>::iterator it;
    irc::message                        *msg;
    int (*funcp)(irc::message *);
    std::map<str, int (*)(irc::message *)>::iterator itmap;

    while (_received.size() != 0 ){
        msg = _received.front();
        if (msg->parse_message() == SUCCESS){
            msg->print();
            itmap = _cmds.find( msg->get_cmd() );
            if (itmap != _cmds.end()){
                std::cout << "found cmd " << itmap->first << std::endl;
                funcp = itmap->second;
                funcp(msg);
            }
        }
        delete msg;
        _received.pop();    
    }
}

// ----- Debug / Print -----
void        irc::server::print_users( void ){
    std::map<int, irc::user*>::iterator it;
    int                              nb = 0;
    
    for (it = _users.begin(); it != _users.end(); it++){
        std::cout << "User No: " << nb << " has fd = " << (*it).first << std::endl;
        nb++;
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

// ----- Init cmd map -----
void       irc::server::init_cmd_map(){
    _cmds.insert(std::make_pair("ADMIN", ft_admin));
    _cmds.insert(std::make_pair("CAP", ft_cap));
    _cmds.insert(std::make_pair("ERROR", ft_error));
    _cmds.insert(std::make_pair("INFO", ft_info));
    _cmds.insert(std::make_pair("INVITE", ft_invite));
    _cmds.insert(std::make_pair("JOIN", ft_join));
    _cmds.insert(std::make_pair("KICK", ft_kick));
    _cmds.insert(std::make_pair("KILL", ft_kill));
    _cmds.insert(std::make_pair("LIST", ft_list));
    _cmds.insert(std::make_pair("MODE", ft_mode));
    _cmds.insert(std::make_pair("NAMES", ft_names));
    _cmds.insert(std::make_pair("NICK", ft_nick));
    _cmds.insert(std::make_pair("NOTICE", ft_notice));
    _cmds.insert(std::make_pair("OPER", ft_oper));
    _cmds.insert(std::make_pair("PASS", ft_pass));
    _cmds.insert(std::make_pair("PART", ft_part));
    _cmds.insert(std::make_pair("PING", ft_ping));
    _cmds.insert(std::make_pair("PONG", ft_pong));
    _cmds.insert(std::make_pair("PRIVMSG", ft_privmsg));
    _cmds.insert(std::make_pair("QUIT", ft_quit));
    _cmds.insert(std::make_pair("STATS", ft_stats));
    _cmds.insert(std::make_pair("TIME", ft_time));
    _cmds.insert(std::make_pair("TOPIC", ft_topic));
    _cmds.insert(std::make_pair("USER", ft_user));
    _cmds.insert(std::make_pair("VERSION", ft_version));
    _cmds.insert(std::make_pair("WHO", ft_who));
    _cmds.insert(std::make_pair("WHOIS", ft_whois));
    _cmds.insert(std::make_pair("WHOWAS", ft_whowas));

}
