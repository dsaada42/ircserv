/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 09:19:34 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/26 09:01:38 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP
# include "ircserv.hpp"
# include "message.hpp"

namespace irc{
    
    class user{
        protected:
            str             _username;
            str             _nickname;
            str             _fullname;
            bool            _oper;
            int             _fd;
            char            _buff[BUFF_SIZE];
            bool            _connected;
            bool            _ping;
            unsigned long   _remain;
            unsigned long   _timestamp; //time in ms set at creation
            
        
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
            const bool  & oper     ( void ) const;
            const int   & fd       ( void ) const;
            const unsigned long & timestamp( void ) const;
            const bool  & connected( void ) const;
            const bool  & ping( void ) const;

        // ----- Setters -----
            void set_username( const str & username );
            void set_nickname( const str & nickname );
            void set_fullname( const str & fullname );
            void set_oper    ( const bool & oper );
            void set_connected( const bool & connected);
            void set_ping( const bool & ping);

        // ----- Read Connexion -----
            int read_connection( void );
            message *extract_message(str delim);
            void new_timestamp(void);
        // ----- Debug -----
            void print(void);
    };

}

#endif