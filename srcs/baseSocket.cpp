/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   baseSocket.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 08:25:37 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/12 10:44:29 by dsaada           ###   ########.fr       */
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
    test_connection(_sock);
}

//----- Destructor -----
irc::baseSocket::~baseSocket(void){}

void irc::baseSocket::test_connection(int to_test ){
    if (to_test < 0){
        std::cerr << "Failed to connect..."  << std::endl;
    }
}

//-----  Getters -----
struct sockaddr_in  irc::baseSocket::get_addr()     { return(_address);    }
int irc::baseSocket::get_sock()                     { return(_sock);       }
int irc::baseSocket::get_connection()               { return(_connection); }