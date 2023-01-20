/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dylan <dylan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 16:23:24 by dylan             #+#    #+#             */
/*   Updated: 2023/01/20 17:10:03 by dylan            ###   ########.fr       */
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
        exit(1);
    }
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
        exit(1);
    }
}

void irc::serverSocket::start_listening(){
    if (listen(_fd, _backlog) < 0){
        perror("Failed to listen on socket");
        exit(1);
    }
}