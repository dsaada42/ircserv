/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 10:47:49 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/19 12:44:57 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ircserv.hpp"

int main(void){
    int testons = 0;
    irc::server serv(6667, "PaSsWoRd!");
    fd_set      *read_sockets;
    fd_set      *write_sockets;

    while (true){
        read_sockets = serv.get_fd_set();
        write_sockets = serv.get_fd_set();
        if (select(FD_SETSIZE, read_sockets, write_sockets, NULL, NULL) < 0 ) {
            perror("select returned an error");
            exit(1);
        }
        for (int i = 0; i < FD_SETSIZE; i++){
            if (FD_ISSET(i, read_sockets)){
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
            else if (FD_ISSET(i, write_sockets)){
                if (testons == 5)
                    exit(1);
                std::cout << "Sending message to established connection " << i << std::endl;
                irc::message test("\r\nNOTICE * Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur sit amet varius tortor, at molestie turpis. Phasellus eget quam ac erat venenatis ornare. Vestibulum a metus ac nulla lacinia sollicitudin eget at risus. Morbi volutpat porttitor turpis, ac faucibus lacus faucibus quis. Morbi nec elementum massa. Quisque malesuada aliquet dui, sit amet auctor dui lacinia nec. In hac habitasse platea dictumst. Quisque egestas dictum ante vel pellentesque. Nulla consequat malesuada tortor, sed posuere diam ornare ut. Donec pellentesque tortor nulla, id aliquet orci iaculis ac. Morbi tempus justo at eros ornare, in posuere leo consectetur. Sed eget eros sed neque tristique egestas sagittis a erat. Aenean eget mauris vel purus ultricies pulvinar sed sit amet tellus. In at blandit dui. Mauris lacinia ipsum eget mauris dictum, dictum congue nunc blandit. Sed scelerisque vulputate eros, in vulputate augue porttitor non. Praesent laoreet quam vel eleifend consequat. Phasellus vitae fermentum ac.\r\n");
                serv.send_message(i, test);
                serv.clear_fd(i);  //used to remove socket from list of file descriptors we are watching
                testons++;
            }
        }
        serv.print_users();
        std::cout << "======================" << std::endl;
        delete read_sockets;
        delete write_sockets;
    }
}