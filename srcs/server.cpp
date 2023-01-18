/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:11:15 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/18 17:01:05 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

// ----- Constructor / Destructor ------
irc::server::server(int port, std::string password) : 
    _port(port), _pass(password), _sock(AF_INET, SOCK_STREAM, TCP, port, INADDR_ANY, BACKLOG){
    //Add _sock to fd_set
    FD_ZERO(&_current_sockets);
    FD_SET(get_sock_fd(), &_current_sockets);   
}

irc::server::~server( void ){
    shutdown(_sock.get_sock(), 2);
    close(_sock.get_sock());
}

// ----- Getters -----
const std::string   & irc::server::get_pass( void ) const               { return(_pass); }
const int           & irc::server::get_port( void ) const               { return(_port); }
const int           & irc::server::get_sock_fd( void ) const            { return(_sock.get_sock()); }
fd_set              irc::server::get_current_sockets( void ) const    { return(_current_sockets); }

// ----- Network -----
void        irc::server::clear_fd(const int & fd)       { FD_CLR(fd, &_current_sockets); }
void        irc::server::accept_connection( void )      {
    int connfd;

    connfd = accept(get_sock_fd(), (struct sockaddr *)NULL, NULL );
    if (connfd < 0){
        std::cerr << "Error accepting connection" << std::endl;
        exit(1);
    }
    FD_SET(connfd, &_current_sockets);
}
