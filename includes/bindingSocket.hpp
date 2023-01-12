/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bindingSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 08:57:28 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/12 09:23:14 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BINDING_SOCKET_HPP
# define BINDING_SOCKET_HPP
# include "baseSocket.hpp"

namespace irc{
    
    class bindingSocket: public baseSocket {
        public:
        //----- Constructor -----
            bindingSocket( int domain, int service, int protocol , int port, u_long interface );
        
        //----- Destructor -----
            ~bindingSocket( void );
        
        //----- Network connection -----
            int network_connect( int sock, SA address );
    };
}

#endif