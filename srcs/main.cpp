/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 10:47:49 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/25 12:07:41 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ircserv.hpp"
#include "server.hpp"

int main(void){
    
    try{
        irc::server serv(6667, "PaSsWoRd!");
        
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);

        serv.run();
    }
    catch (exitException){   
        std::cout << "Server exited" << std::endl;
        return (SUCCESS);
    }
}