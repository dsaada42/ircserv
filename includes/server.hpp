/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dylan <dylan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 09:50:40 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/20 17:05:41 by dylan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# include "ircserv.hpp"

namespace irc{
    
    class user;
    class message;
    class serverSocket;
    
    class server{
        private:
            std::string                             _server_name;
            int                                     _port;
            std::string                             _pass;
            std::map<int, user*>                    _users;
            std::map<std::string, channel*>         _channels;
            std::queue<message*>                    _messages;
            irc::serverSocket                       _sock;
            fd_set                                  read_sockets;
            fd_set                                  write_sockets;
            fd_set                                  except_sockets;
            
        public:
            server(int port = 6667, std::string password = "");
            ~server( void );

        //----- Getters -----
            const int   & port(void) const;
            const int   & sock_fd(void) const;
            
        // ----- Debug / Print -----
            void        print_users( void );

        // ----- Main loop -----
            int         run( void );

        private:
        // ----- Network -----
            void        accept_connection(void);
            void        read_connection(const int & fd);
            void        send_message(const int & fd, irc::message msg);
            void        handle_read_set(void);
            void        handle_except_set(void);
            void        handle_write_set(void);
        // ----- Select helper -----
            void        update_sets(void);

           
    };
}

#endif