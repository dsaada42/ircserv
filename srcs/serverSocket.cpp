/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 16:23:24 by dylan             #+#    #+#             */
/*   Updated: 2023/02/03 14:09:05 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "serverSocket.hpp"

// ----- Constuctor -----
irc::serverSocket::serverSocket( int domain, int service, int protocol , int port, u_long interface, int backlog ) : _backlog(backlog) {
    _address.sin_family = domain;
    _address.sin_port = htons(port);
    _address.sin_addr.s_addr = htonl(interface);
    _fd = socket(domain, service, protocol);
    if (_fd < 0){
        perror("Failed to create socket...");
        throw(exitException());
    }
    const int enable = 1;
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");
    bind_socket();
    start_listening();
}

// ----- Destructor -----
irc::serverSocket::~serverSocket(void){
    if (_fd > 0){
        shutdown(_fd, 2);
        close(_fd);
    }
}

// -----  Getters -----
const int & irc::serverSocket::fd() const { return(_fd);    }

// ----- Bind socket -----
void irc::serverSocket::bind_socket(){
    if (bind(_fd, (struct sockaddr *)&_address, sizeof(_address)) < 0){
        perror("Failed to bind socket");
        throw(exitException());
    }
}

void irc::serverSocket::start_listening(){
    if (listen(_fd, _backlog) < 0){
        perror("Failed to listen on socket");
        throw(exitException());
    }
}