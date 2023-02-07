/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 09:50:40 by dsaada            #+#    #+#             */
/*   Updated: 2023/02/07 17:30:13 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# include "ircserv.hpp"
# include "serverSocket.hpp"
# include "user.hpp"
# include "channel.hpp"
# include "message.hpp"
# include "adminUser.hpp"
# include "send.hpp"
# include "utils.hpp"


namespace irc{
    
    class server{
        public:
            typedef void (server::*srv_func)(message *);
        private:
            str                                 _server_name;
            int                                 _port;
            str                                 _pass;
            std::map<str, srv_func>             _cmds;
            std::map<int, user*>                _users;
            std::map<str, channel*>             _channels;
            std::queue<message*>                _received;
            std::queue<message*>                _messages;
            irc::serverSocket                   _sock;
            fd_set                              read_sockets;
            fd_set                              write_sockets;
            fd_set                              except_sockets;
            irc::adminUser                      _admin;
            unsigned long                       _creation_time;
            str                                 _creation_date;
            
        public:
        // ----- Constructor / Destructor -----
            server(int port = 6667, str password = "");
            ~server( void );

        //----- Getters -----
            const int   & port(void) const;
            const int   & sock_fd(void) const;
    
        // ----- Main loop -----
            int         run( void );

        private:
        // ----- Network -----
            void        accept_connection(void);
            int         handle_user_connection(user * current);
            void        say_welcome(user * new_user);
        // ----- Timeout / load handler -----
            unsigned long get_time_ms(void);
            void        handle_users_timeout(void);
        // ----- Select helper -----
            void        update_sets(void);

        //====== READ / REPLY ======
        
        // ----- Read Handler -----
            void        handle_read_set(void);

        // ----- Write Handler
            void        handle_write_set(void);

        //====== INIT / MEMORY MANAGEMENT ======
            
        // ----- Memory handler ----- 
            void        delete_user(user * el, str reason);
            void        delete_user(const str & nick);
            void        delete_all_users(void);
            void        delete_channel(const str & name);
            void        delete_all_channels(void);
            void        delete_all_messages(void);
            void        delete_all_received(void);
        // ----- Init cmd map -----
            void        init_cmd_map(void);
        // ----- Clear useless -----
            void        clear_useless( void );

        //====== IRC LOGIC / RECEIVED COMMANDS ======

        // ----- Find user by -----
            irc::user   *find_user_by_fd(const int & fd);
            irc::user   *find_user_by_nick(const str & nick);
            irc::channel*find_channel_by_name(const str & name);
            str          user_prefix(irc::user *usr);
        // ----- Is ? ------
            bool        check_nickname_rules(const str & nick);
            bool        check_channel_rules(const str & chan);
            bool        is_a_nickname(const str & nick);
            bool        is_a_channel(const str & chan);
        // ----- Message Interpreter + Reply generator -----
            void        interprete_and_reply(void);
        // ----- Received message treatement -----
            void        ft_admin( irc::message * );
            void        ft_cap( irc::message * );
            void        ft_info( irc::message * );
            void        ft_invite( irc::message * );
            void        ft_join( irc::message * );
            void        ft_kick( irc::message * );
            void        ft_kill( irc::message * );
            void        ft_list( irc::message * );
            void        ft_mode(irc::message *);
            void        ft_names(irc::message *);
            void        ft_nick(irc::message *);
            void        ft_notice(irc::message *);
            void        ft_oper(irc::message *);
            void        ft_pass(irc::message *);
            void        ft_part(irc::message *);
            void        ft_ping(irc::message *);
            void        ft_pong(irc::message *);
            void        ft_privmsg(irc::message *);
            void        ft_quit(irc::message *);
            void        ft_stats(irc::message *);
            void        ft_time(irc::message *);
            void        ft_topic(irc::message *);
            void        ft_user(irc::message *);
            void        ft_version(irc::message *);
            void        ft_who(irc::message *);
            int         who_one_nick(irc::user *sender, str nick);
            void        ft_whois(irc::message *);
            void        ft_whowas(irc::message *);
            //disabled
            void        ft_users(irc::message *);
            void        ft_summon(irc::message *);
            
        //====== TESTING FUNCTIONS ======
            int         manual_entry( void );     
            int         send_message(const int & fd, irc::message msg);
            void        print_users( void );
    };
}

#endif