/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 09:50:40 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/23 08:56:54 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# include "ircserv.hpp"
# include "serverSocket.hpp"
# include "user.hpp"
# include "channel.hpp"
# include "message.hpp"

namespace irc{
    
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
            int         send_message(const int & fd, irc::message msg);
            void        handle_read_set(void);
            void        handle_write_set(void);
        // ----- Timeout / load handler -----
            void        handle_users_timeout(void);
        // ----- Select helper -----
            void        update_sets(void);

        // ----- Memory handler ----- --> to a class?
            void        delete_user(user * el);
            void        delete_user(const std::string & nick);
            void        delete_all_users(void);
            void        delete_channel(const std::string & name);
            void        delete_all_channels(void);
            void        delete_all_messages(void);

           
    };
}

#endif