/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 09:19:34 by dsaada            #+#    #+#             */
/*   Updated: 2023/02/07 17:04:47 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP
# include "ircserv.hpp"
# include "message.hpp"
# include "utils.hpp"

namespace irc{
    
    class user{
        protected:
            str             _username;
            str             _nickname;
            str             _oldnick;
            str             _fullname;
            int             _fd;
            char            _buff[BUFF_SIZE];
            bool            _connected;
            bool            _ping;
            unsigned long   _remain;
            unsigned long   _timestamp;
            unsigned long   _creation_time; //time in ms set at creation
            bool            _pass;
            str             _mode;
            unsigned long   _sent;
            unsigned long   _sentbytes;
            unsigned long   _received;
            unsigned long   _receivedbytes;
        
        public:
        // ----- Constructors -----
            user(int fd, unsigned long timestamp);
            user(const user & x);
        // ----- Destructor -----
            virtual ~user( void );
        // ----- Operator= -----
            user& operator= (const user & x);

        // ----- Getters ------
            const str   & username ( void ) const;
            const str   & nickname ( void ) const;
            const str   & fullname ( void ) const;
            const int   & fd       ( void ) const;
            const unsigned long & timestamp( void ) const;
            const bool  & connected( void ) const;
            const bool  & ping( void ) const;
            const bool  & pass( void ) const;
            const str   & mode( void ) const;
            const str   & oldnick( void ) const;
            const unsigned long & creation_time( void ) const;

        // ----- Setters -----
            void set_username( const str & username );
            void set_nickname( const str & nickname );
            void set_fullname( const str & fullname );
            void set_connected( const bool & connected);
            void set_ping( const bool & ping);
            void set_pass( const bool & pass);


        // ----- Mode -----
            bool is_mode(char c);
            void change_mode(char c, bool plus);
        // ----- Read Connexion -----
            int read_connection( void );
            message *extract_message(str delim);
            void new_timestamp(void);
        // ----- Stats -----
            void incr_sent_cmd(void);
            void incr_sent_bytes(int n);
            str connection_stats(void);
        // ----- Debug -----
            void print(void);
    };

}

#endif