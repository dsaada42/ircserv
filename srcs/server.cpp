/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:11:15 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/18 14:49:51 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

// ----- Constructor / Destructor ------
irc::server::server(int port, std::string password) : 
    _port(port), _pass(password), _sock(AF_INET, SOCK_STREAM, TCP, port, INADDR_ANY, BACKLOG){}

irc::server::~server( void ){
    shutdown(_sock.get_sock(), 2);
    close(_sock.get_sock());
}

// ----- Getters -----
std::string & irc::server::get_pass( void )     { return(_pass); }
int         & irc::server::get_port( void )     { return(_port); }
int         & irc::server::get_sock_fd( void )  { return(_sock.get_sock()); }

