/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   baseSocket.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 08:25:37 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/18 16:57:08 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "baseSocket.hpp"

//----- Default constructor -----
irc::baseSocket::baseSocket(int domain, int service, int protocol, int port, u_long interface){
    //Defining address structure
    _address.sin_family = domain;
    _address.sin_port = htons(port);
    _address.sin_addr.s_addr = htonl(interface);
    //Establish connection
    _sock = socket(domain, service, protocol);
    //Test socket
    if (_sock < 0){
        perror("Failed to create socket...");
        exit(1);
    }
}

//----- Destructor -----
irc::baseSocket::~baseSocket(void){}

//-----  Getters -----
struct      sockaddr_in  irc::baseSocket::get_addr() const  { return(_address);    }
const int & irc::baseSocket::get_sock() const               { return(_sock);       }
const int & irc::baseSocket::get_connection() const         { return(_connection); }