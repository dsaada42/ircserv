/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:11:15 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/23 15:08:06 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

// ----- Constructor / Destructor ------
irc::server::server(int port, str password) : 
    _port(port), _pass(password), _sock(AF_INET, SOCK_STREAM, TCP, port, INADDR_ANY, BACKLOG){
        // init_cmd_map();
    }

irc::server::~server( void ){
    delete_all_channels();
    delete_all_users();
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
        exit(1);
    }
    irc::user * new_user = new irc::user(newfd);
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
    irc::user                           *user;
    
    //cas 1 : readable server socket
    if (FD_ISSET(sock_fd(), &read_sockets)){
        std::cout << "New connection incoming" << std::endl;
        accept_connection();
    }
    it = _users.begin();
    while ( it != _users.end() ){
        user = (*it).second;
        // this allows deletion of user in the middle of the loop
        it++; 
        //cas 2 : la socket d'un des users est readable
        if (FD_ISSET(user->fd(), &read_sockets)){
            std::cout << "Received message from established connection" << std::endl;
            if (user->read_connection() == FAILURE){
                std::cout << "deleting user because connection lost" << std::endl;   
                delete_user(user);
            }
        }
        //cas 3 : exception sur la connexion , on ferme 
        else if (FD_ISSET(user->fd(), &except_sockets)){
            std::cout << "Exception occured on connexion with user, closing connexion" << std::endl;
            delete_user(user);
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
            exit(1);
        }
        handle_read_set();
        //interprete messages
        handle_write_set();

        handle_users_timeout();
    }
}

// ----- Timeout / load handler -----
void       irc::server::handle_users_timeout(void){}

// ----- Select helper -----
 void       irc::server::update_sets( void )           {
    std::map<int, irc::user*>::iterator it = _users.begin();
    
    FD_ZERO(&read_sockets);
    FD_ZERO(&write_sockets);
    FD_ZERO(&except_sockets);
    FD_SET(sock_fd(), &read_sockets);
    FD_SET(sock_fd(), &write_sockets);
    FD_SET(sock_fd(), &except_sockets);
    for (; it != _users.end(); it++){
        FD_SET((*it).first, &read_sockets);
        FD_SET((*it).first, &write_sockets);
        FD_SET((*it).first, &except_sockets);
    }
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

// ----- Debug / Print -----
void        irc::server::print_users( void ){
    std::map<int, irc::user*>::iterator it;
    int                              nb = 0;
    
    for (it = _users.begin(); it != _users.end(); it++){
        std::cout << "User No: " << nb << " has fd = " << (*it).first << std::endl;
        nb++;
    }
}

// // ----- Init cmd map -----
// void       irc::server::init_cmd_map(){
//     int i;
//     for (i = 0; i < sizeof(cmd_list)/sizeof(s_cmd); i++) {
//         func_map[cmd_list[i].name] = cmd_list[i].func;
//     }
//     std::cout << "initialized " << i << " commands" << std::endl;
// }
