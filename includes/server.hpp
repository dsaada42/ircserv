/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 09:50:40 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/19 11:33:43 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# include "ircserv.hpp"

namespace irc{
    
    class user;
    class message;
    class server{
        private:
            int                         _port;
            std::string                 _pass;
            std::vector<irc::user>      _users;
            std::vector<irc::channel>   _channels;
            irc::listeningSocket        _sock;
            fd_set                      _current_sockets;
            
        public:
            server(int port = 6667, std::string password = "");
            ~server( void );

        //----- Getters -----
            const std::string & get_pass(void) const;
            const int   & get_port(void) const;
            const int   & get_sock_fd(void) const;
            fd_set        get_current_sockets(void) const;
            
        // ----- Network -----
            void        accept_connection(void);
            void        clear_fd(const int & fd);
            void        read_connection(const int & fd);
            void        send_message(const int & fd, irc::message msg);

        // ----- Debug / Print -----
            void        print_channel_users( irc::channel chan );
            void        print_users( void );
    };
}

#endif