/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 10:47:49 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/19 11:34:13 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ircserv.hpp"

int main(void){
    irc::server serv(6667, "PaSsWoRd!");
    fd_set      read_sockets;
    fd_set      write_sockets;

    while (true){
        read_sockets = serv.get_current_sockets();
        write_sockets = serv.get_current_sockets();
        if (select(FD_SETSIZE, &read_sockets, &write_sockets, NULL, NULL) < 0 ) {
            perror("select returned an error");
            exit(1);
        }
        for (int i = 0; i < FD_SETSIZE; i++){
            if (FD_ISSET(i, &read_sockets)){
                if (i == serv.get_sock_fd()){
                    std::cout << "New connection incoming" << std::endl;
                    serv.accept_connection();
                }
                else {
                    std::cout << "Received message from established connection" << std::endl;
                    serv.read_connection(i);
                    serv.clear_fd(i);  //used to remove socket from list of file descriptors we are watching
                }
            }
            else if (FD_ISSET(i, &write_sockets)){
                std::cout << "Sending message to established connection " << i << std::endl;
                irc::message test("\r\nNOTICE wesh la cite\r\n");
                serv.send_message(i, test);
                serv.clear_fd(i);  //used to remove socket from list of file descriptors we are watching
            }
        }
        serv.print_users();
        std::cout << "======================" << std::endl;
    }
}