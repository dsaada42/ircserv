/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 09:19:34 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/24 17:55:49 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP
# include "ircserv.hpp"
# include "message.hpp"

namespace irc{
    
    class user{
        protected:
            str                     _username;
            str                     _nickname;
            str                     _fullname;
            std::queue<message*>    _messages;
            bool                    _oper;
            int                     _fd;
            char                    _buff[BUFF_SIZE];
            unsigned long           _remain;

        
        public:
        // ----- Constructors -----
            user(int fd);
            user(str nickname, str username, str fullname, bool oper, int fd);
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
            std::queue<message*> & messages( void );

        // ----- Setters -----
            void set_username( const str & username );
            void set_nickname( const str & nickname );
            void set_fullname( const str & fullname );
            void set_oper    ( const bool & oper );

        // ----- Read Connexion -----
            int read_connection( void );
            void extract_messages(str delim);


            
    };

}

#endif