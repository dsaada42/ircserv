/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/08 09:06:55 by dylan             #+#    #+#             */
/*   Updated: 2023/01/17 08:43:27 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
# define IRCSERV_HPP

# include <cstdlib>
# include <iostream>
// ---- Sockets -----
# include "baseSocket.hpp"
# include "bindingSocket.hpp"
# include "listeningSocket.hpp"
# include "connectingSocket.hpp"
// ----- Select -----
# include <sys/select.h>
// ----- IRC Objects -----
# include "server.hpp"
# include "channel.hpp"
# include "user.hpp"
# include "message.hpp"

#define SERVER_PORT 6667
#define TCP 0
#define BACKLOG 10
#define BUFF_SIZE 1024
#endif