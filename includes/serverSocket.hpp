/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dylan <dylan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 16:22:44 by dylan             #+#    #+#             */
/*   Updated: 2023/01/20 17:13:44 by dylan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_SOCKET_HPP
# define SERVER_SOCKET_HPP
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <cstdlib>
# include <stdio.h>

namespace irc{

    class serverSocket{
        private:
            int _backlog;
            int _fd;
            struct sockaddr_in  _address;
        
        public:
        // ----- Constructor -----
            serverSocket(int domain, int service, int protocol , int port, u_long interface, int backlog);
        // ----- Destructor -----
            virtual ~serverSocket(void);
        // ----- fd -----
            const int & fd( void ) const;
        private:
        // ----- Bind -----
            void bind_socket( void );

        // ----- Listen -----
            void start_listening( void );   
    
    };
}

#endif