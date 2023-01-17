/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listeningSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 10:04:32 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/17 09:08:52 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "listeningSocket.hpp"

//----- Constructor -----
irc::listeningSocket::listeningSocket(int domain, int service, int protocol, int port, u_long interface, int backlog)
    : bindingSocket(domain, service, protocol, port, interface){
    _backlog = backlog;
    // Start listening
    start_listening();
    // check listening status
    if (_listening < 0){
        perror("Failed to listen on socket");
        exit(1);
    }
}

//----- Destructor -----
irc::listeningSocket::~listeningSocket(void){}

//----- Listening -----
void irc::listeningSocket::start_listening( void ){
    _listening = listen(_sock, _backlog);
}