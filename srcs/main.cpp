/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 10:47:49 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/12 11:23:53 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ircserv.hpp"
#define SA struct sockaddr
#include <unistd.h>
#include <strings.h>

int main (void){

    //----- Create Socket setup for listening -----
    //AF_INET = Address Family - Internet
    //SOCK_STREAM = Stream Socket
    //TCP = protocol used (defined 0)
    //SERVER_PORT = port open to listen (defined 6667)
    //INADDR_ANY = bind to any address   
    //BACKLOG = max number of queued connections (defined 10 but is architecture dependant and could be limited to 5)
    irc::listeningSocket sock(AF_INET, SOCK_STREAM, TCP, SERVER_PORT, INADDR_ANY, BACKLOG);
    int connfd;
    std::string         result;
    //BUFF_SIZE = maximum length of an IRC message (512 max for prefix, 512 max for message + commands) (defined 1024)
    char                buffer[BUFF_SIZE];
    
    while (1){
        int n = 0;

        std::cout << "Waiting for a connection on port " << SERVER_PORT << std::endl;
        //Accept will block until incoming connexion 
        connfd = accept(sock.get_sock(), (SA *) NULL, NULL);
        if (connfd < 0){
            std::cerr << "Error accepting connection" << std::endl;
            return(-1);
        }
        std::cout << "received message" << std::endl;
        while ((n = read(connfd, buffer, sizeof(buffer) - 1)) > 0){
            result += buffer;
        }
        bzero(&buffer, sizeof(buffer));
        std::cout << "[Message] " << result << " [End of Message]" << std::endl;
        close(connfd);
        result.clear();
    }
}