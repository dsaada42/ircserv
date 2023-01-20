/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dylan <dylan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/08 09:06:55 by dylan             #+#    #+#             */
/*   Updated: 2023/01/20 17:13:49 by dylan            ###   ########.fr       */
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
# include <queue>
# include <unistd.h>
# include <strings.h>
// ---- Sockets -----
# include "serverSocket.hpp"
// ----- Select -----
# include <sys/select.h>
// ----- IRC Objects -----
# include "channel.hpp"
# include "user.hpp"
# include "message.hpp"
# include "server.hpp"

#endif