/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connectingSocket.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 09:16:46 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/12 10:26:05 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTING_SOCKET_HPP
# define CONNECTING_SOCKET_HPP
# include "baseSocket.hpp"

namespace irc{

    class connectingSocket: public baseSocket {
        public:
        //----- Constructor -----
            connectingSocket(int domain, int service, int protocol, int port, u_long interface);
        
        //----- Destructor -----
            ~connectingSocket( void );

        //----- Network Connection -----
            int network_connect(int sock, SA address);
    };
        
}

#endif