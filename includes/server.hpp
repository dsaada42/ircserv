/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 09:50:40 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/18 15:21:21 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# include "ircserv.hpp"

namespace irc{
    
    class user;
    class server{
        private:
            int                          _port;
            std::string                  _pass;
            //replaces sets by map <user.username, user>
            //map<channel.name , channel>
            std::vector<irc::user>      _users;
            std::vector<irc::channel>   _channels;
            irc::listeningSocket        _sock;
        public:
            server(int port = 6667, std::string password = "");
            ~server( void );

        //----- Getters -----
            std::string & get_pass(void);
            int         & get_port(void);
            int         & get_sock_fd(void);
            
    };
}

#endif