/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/08 09:06:55 by dylan             #+#    #+#             */
/*   Updated: 2023/01/23 07:52:03 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
# define IRCSERV_HPP
#define SERVER_PORT 6667
#define TCP 0
#define BACKLOG 10
#define BUFF_SIZE 1024
#define SUCCESS 0
#define FAILURE 1

# include <cstdlib>
# include <iostream>
# include <vector>
# include <map>
# include <queue>
# include <unistd.h>
# include <strings.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <cstdlib>
# include <stdio.h>
// ----- Select -----
# include <sys/select.h>


#endif