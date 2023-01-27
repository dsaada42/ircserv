/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 10:47:49 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/27 10:31:07 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ircserv.hpp"
#include "server.hpp"

int main(int argc, char **argv){
    int port;

    if (argc != 3){    
        std::cout << "Wrong number of arguments:\n./ircserv <port> <password>" << std::endl;
        return(FAILURE);
    }
    port = atoi(argv[1]);   
    if (port == 0){
        std::cout << "port should be a numerical value" << std::endl;
        return(FAILURE);
    }
    
    try{
        irc::server serv(port, argv[2]);
        
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);

        serv.run();
    }
    catch (exitException){   
        std::cout << "Server exited" << std::endl;
        return (SUCCESS);
    }
}