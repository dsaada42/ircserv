/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/08 09:06:55 by dylan             #+#    #+#             */
/*   Updated: 2023/02/07 11:31:00 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
# define IRCSERV_HPP
#define SERVER_PORT 6667
#define SERVER_NAME "shittyIRC"
#define SERVER_VERSION "V1.0"
#define UMODES "ios"
#define CMODES "ptiok"
#define OPER_PASSWORD "Metsmoioperwesh!"
#define TCP 0
#define BACKLOG 10
#define BUFF_SIZE 1024
#define SUCCESS 0
#define FAILURE 1
#define PING_TRIGGER_TIME 20000
#define DISCONNECT_TRIGGER_TIME 30000
#define CONNECTION_TIMEOUT 30000
#include <sstream>

//macro to convert int to string
#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

# include <cstdlib>
# include <algorithm>
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
# include <ctime>
# include <stdio.h>
# include <csignal>
# include <exception>
# include <utility>
# include <sys/time.h>
# include <iomanip>
#include <ctype.h>

// ----- Select -----
# include <sys/select.h>

// ----- Typedefs -----
typedef std::string str;

// ----- Exceptions -----
class exitException : public std::exception {
    public:
        virtual const char* what() const throw(){
            return ("");
        };
};

#endif