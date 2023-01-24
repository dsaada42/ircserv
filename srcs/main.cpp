/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 10:47:49 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/24 09:44:42 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ircserv.hpp"
#include "server.hpp"

int main(void){
    
    irc::server serv(6667, "PaSsWoRd!");
    
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    try{
        serv.run();
    }
    catch (exitException){   
        std::cerr << "Server exited" << std::endl;
        return (SUCCESS);
    }
}