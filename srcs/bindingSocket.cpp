/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bindingSocket.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 09:03:16 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/17 09:03:37 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bindingSocket.hpp"

//----- Constructor -----
irc::bindingSocket::bindingSocket(int domain, int service, int protocol, int port, u_long interface)
    : baseSocket(domain, service, protocol, port, interface){
    //Binding socket
    _connection = network_connect(_sock, _address);
    //test binding success
    if (_connection < 0){
        perror("Failed to bind socket");
        exit(1);
    }
}

//----- Destructor -----
irc::bindingSocket::~bindingSocket(void){}

//----- Network Connection -----
int irc::bindingSocket::network_connect(int sock, SA address){
    return bind(sock, (struct sockaddr *)&address, sizeof(address));
}