/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 10:47:49 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/18 17:03:51 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ircserv.hpp"

int handle_connection(int connfd){
    std::string result;
    char        buffer[BUFF_SIZE];
    
    read(connfd, buffer, sizeof(buffer) - 1); // blocking 
    result += buffer;
    bzero(&buffer, sizeof(buffer));
    std::cout << "[Message] " << result << " [End of Message]" << std::endl;
    close(connfd);
    result.clear();

    return (0);
}

int main(void){
    irc::server serv(6667, "PaSsWoRd!");
    fd_set      ready_sockets;

    while (true){
        ready_sockets = serv.get_current_sockets();
        if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0 ) {
            perror("select returned an error");
            exit(1);
        }
        for (int i = 0; i < FD_SETSIZE; i++){
            if (FD_ISSET(i, &ready_sockets)){
                if (i == serv.get_sock_fd()){
                    std::cout << "New connection incoming" << std::endl;
                    serv.accept_connection();
                }
                else {
                    std::cout << "Received message from established connection" << std::endl;
                    handle_connection(i);
                    serv.clear_fd(i);  //used to remove socket from list of file descriptors we are watching
                }
            }
        }
    }
}