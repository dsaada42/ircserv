/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connectingSocket.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 09:19:38 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/17 09:09:08 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "connectingSocket.hpp"

//----- Constructor -----
irc::connectingSocket::connectingSocket(int domain, int service, int protocol, int port, u_long interface)
    : baseSocket(domain, service, protocol, port, interface){
    //Connecting socket
    _connection = network_connect(_sock, _address);
    if (_connection < 0){
        perror("Failed to connect socket");
        exit(1);
    }
}

//----- Destructor -----
irc::connectingSocket::~connectingSocket(void){}

//----- Network connection -----
int irc::connectingSocket::network_connect(int sock, SA address){
    return connect(sock, (struct sockaddr *)&address, sizeof(address));
}