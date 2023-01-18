/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   baseSocket.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 08:12:04 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/18 14:50:20 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASE_SOCKET_HPP
# define BASE_SOCKET_HPP
# include <sys/socket.h>
# include <netinet/in.h>
# include <iostream>
# include <cstdlib>
# include <stdio.h>
# include <errno.h>

namespace irc{
    
    class baseSocket{
        public:
            typedef struct sockaddr_in         SA;
            
        protected:
            int _sock;
            int _connection;
            SA  _address;
            
        public:
        //----- Constructor / Destructor -----
            baseSocket(int domain, int service, int protocol , int port, u_long interface);
            virtual ~baseSocket(void);
        
        //----- Connections -----
            virtual int network_connect(int sock, SA address) = 0;
            
        //----- Getters -----
            int & get_sock();
            SA  get_addr();
            int & get_connection();
            
               
    };

}

#endif