/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/08 09:06:55 by dylan             #+#    #+#             */
/*   Updated: 2023/01/19 16:34:27 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
# define IRCSERV_HPP
#define SERVER_PORT 6667
#define TCP 0
#define BACKLOG 10
#define BUFF_SIZE 1024

# include <cstdlib>
# include <iostream>
# include <vector>
# include <map>
# include <unistd.h>
# include <strings.h>
// ---- Sockets -----
# include "baseSocket.hpp"
# include "bindingSocket.hpp"
# include "listeningSocket.hpp"
# include "connectingSocket.hpp"
// ----- Select -----
# include <sys/select.h>
// ----- IRC Objects -----
# include "channel.hpp"
# include "user.hpp"
# include "message.hpp"
# include "server.hpp"




#endif