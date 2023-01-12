/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 10:00:56 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/12 10:26:23 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENING_SOCKET_HPP
# define LISTENING_SOCKET_HPP
# include "bindingSocket.hpp"

namespace irc{

    class listeningSocket: public bindingSocket{
        private:
            int _backlog;
            int _listening;
        
        public:
        //----- Constructor / Destructor -----
            listeningSocket(int domain, int service, int protocol , int port, u_long interface, int backlog);
            virtual ~listeningSocket(void);

        //----- Listening -----
            void start_listening( void );   
    
        };
}

#endif