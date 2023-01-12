/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bindingSocket.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 09:03:16 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/12 10:44:33 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bindingSocket.hpp"

//----- Constructor -----
irc::bindingSocket::bindingSocket(int domain, int service, int protocol, int port, u_long interface)
    : baseSocket(domain, service, protocol, port, interface){
    //Binding socket
    _connection = network_connect(_sock, _address);
    //test binding success
    test_connection(_connection);
}

//----- Destructor -----
irc::bindingSocket::~bindingSocket(void){}

//----- Network Connection -----
int irc::bindingSocket::network_connect(int sock, SA address){
    return bind(sock, (struct sockaddr *)&address, sizeof(address));
}