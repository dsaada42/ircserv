/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:11:15 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/21 12:14:37 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

// ----- Constructor / Destructor ------
irc::server::server(int port, std::string password) : 
    _port(port), _pass(password), _sock(AF_INET, SOCK_STREAM, TCP, port, INADDR_ANY, BACKLOG){
    
}

irc::server::~server( void ){}

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

void        irc::server::send_message(const int & fd, irc::message msg){
    std::string result;
    
    result = msg.get_message();
    write(fd, result.c_str(), result.size());
}

// ----- Exception handler -----
void        irc::server::handle_except_set(void){}

// ----- Read Handler -----
void        irc::server::handle_read_set( void ){
    std::map<int, irc::user*>::iterator it;
    irc::user                           *user;
    
    //cas 1 : readable server socket
    if (FD_ISSET(sock_fd(), &read_sockets)){
        std::cout << "New connection incoming" << std::endl;
        accept_connection();
    }
    //cas 2 : la socket d'un des users est readable
    for (it = _users.begin(); it != _users.end(); it++){
        user = (*it).second;
        if (FD_ISSET(user->fd(), &read_sockets)){
            std::cout << "Received message from established connection" << std::endl;
            user->read_connection();
        }
    }
}

// ----- Write Handler -----
void        irc::server::handle_write_set(void){}

// ----- Main loop -----
int         irc::server::run( void ){

    while( true ){
        update_sets();
        
        if (select(FD_SETSIZE, &read_sockets, &write_sockets, &except_sockets, NULL) < 0 ) {
            perror("select returned an error");
            exit(1);
        }
        handle_except_set(); // mysterieux
        handle_read_set();
        handle_write_set();
    }
}

// ----- Select helper -----
 void              irc::server::update_sets( void )           {
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

// ----- Debug / Print -----
void        irc::server::print_users( void ){
    std::map<int, irc::user*>::iterator it;
    int                              nb = 0;
    
    for (it = _users.begin(); it != _users.end(); it++){
        std::cout << "User No: " << nb << " has fd = " << (*it).first << std::endl;
        nb++;
    }
}
