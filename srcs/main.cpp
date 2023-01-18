/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 10:47:49 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/18 14:43:19 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ircserv.hpp"
#define SA struct sockaddr
#include <unistd.h>
#include <strings.h>

int accept_new_connection(int sock){
    int connfd;
    
    //Accept will block until incoming connexion 
    connfd = accept(sock, (SA *) NULL, NULL);
    if (connfd < 0){
        std::cerr << "Error accepting connection" << std::endl;
        exit(1);
    }
    return (connfd);
}

int handle_connection(int connfd){
    std::string result;
    //BUFF_SIZE = maximum length of an IRC message (512 max for prefix, 512 max for message + commands) (defined 1024)
    char        buffer[BUFF_SIZE];
    
    read(connfd, buffer, sizeof(buffer) - 1); // blocking 
    result += buffer;
    bzero(&buffer, sizeof(buffer));
    std::cout << "[Message] " << result << " [End of Message]" << std::endl;
    close(connfd);
    result.clear();

    return (0);
}

// int main (void){

//     //----- Create Socket setup for listening -----
//     //AF_INET = Address Family - Internet
//     //SOCK_STREAM = Stream Socket
//     //TCP = protocol used (defined 0)
//     //SERVER_PORT = port open to listen (defined 6667)
//     //INADDR_ANY = bind to any address   
//     //BACKLOG = max number of queued connections (defined 10 but is architecture dependant and could be limited to 5)
//     irc::listeningSocket        sock(AF_INET, SOCK_STREAM, TCP, SERVER_PORT, INADDR_ANY, BACKLOG);
//     int                         connfd;
//     fd_set                      current_sockets;
//     fd_set                      ready_sockets;

//     FD_ZERO(&current_sockets);
//     FD_SET(sock.get_sock(), &current_sockets);

//     while (true){
//         ready_sockets = current_sockets;
//         // 0: number of sockets , 1: sockets ready to read, 2: sockets ready to write, 3: socket errors, 4: optional timeout
//         if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0 ) {
//             perror("select returned an error");
//             shutdown(sock.get_sock(), 2);
//             close(sock.get_sock());
//             exit(1);     
//         }
//         //now the fdset is modified and we get only the ones ready:
//         for (int i = 0; i < FD_SETSIZE; i++){
//             if (FD_ISSET(i, &ready_sockets)){
//                 if (i == sock.get_sock()){
//                     //new connection incoming
//                     std::cout << "New connection incoming" << std::endl;
//                     connfd = accept_new_connection(sock);
//                     FD_SET(connfd, &current_sockets);
//                 }
//                 else {
//                     std::cout << "Received message from established connection" << std::endl;
//                     handle_connection(i);
//                     FD_CLR(i, &current_sockets);  //used to remove socket from list of file descriptors we are watching
//                 }
//             }
//         }
//     }
//     shutdown(sock.get_sock(), 2);
//     close(sock.get_sock());
// }

int main(void){
    irc::server serv(6667, "PaSsWoRd!");
    int         connfd;
    fd_set      current_sockets;
    fd_set      ready_sockets;

    FD_ZERO(&current_sockets);
    FD_SET(serv.get_sock_fd(), &current_sockets);

    while (true){
        ready_sockets = current_sockets;
        // 0: number of sockets , 1: sockets ready to read, 2: sockets ready to write, 3: socket errors, 4: optional timeout
        if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0 ) {
            perror("select returned an error");
            // shutdown(serv.get_sock_fd(), 2);
            // close(serv.get_sock_fd());
            exit(1);     
        }
        //now the fdset is modified and we get only the ones ready:
        for (int i = 0; i < FD_SETSIZE; i++){
            if (FD_ISSET(i, &ready_sockets)){
                if (i == serv.get_sock_fd()){
                    //new connection incoming
                    std::cout << "New connection incoming" << std::endl;
                    connfd = accept_new_connection(serv.get_sock_fd());
                    FD_SET(connfd, &current_sockets);
                }
                else {
                    std::cout << "Received message from established connection" << std::endl;
                    handle_connection(i);
                    FD_CLR(i, &current_sockets);  //used to remove socket from list of file descriptors we are watching
                }
            }
        }
    }
    // shutdown(serv.get_sock_fd(), 2);
    // close(serv.get_sock_fd());
}