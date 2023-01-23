/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 09:19:34 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/23 08:30:02 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP
# include "ircserv.hpp"
# include "message.hpp"

namespace irc{
    
    class user{
        private:
            std::string             _username;
            std::string             _nickname;
            std::string             _fullname;
            std::queue<message*>    _messages;
            bool                    _oper;
            int                     _fd;
            char                    _buff[BUFF_SIZE];

        
        public:
        // ----- Constructors -----
            user(int fd);
            user(std::string nickname, std::string username, std::string fullname, bool oper, int fd);
            user(const user & x);
        // ----- Destructor -----
            ~user( void );
        // ----- Operator= -----
            user& operator= (const user & x);

        // ----- Getters ------
            const std::string & username ( void ) const;
            const std::string & nickname ( void ) const;
            const std::string & fullname ( void ) const;
            const bool        & oper     ( void ) const;
            const int         & fd       ( void ) const;

        // ----- Setters -----
            void set_username( const std::string & username );
            void set_nickname( const std::string & nickname );
            void set_fullname( const std::string & fullname );
            void set_oper    ( const bool & oper );

        // ----- Read Connexion -----
            int read_connection( void );
            void extract_messages(void);


            
    };

}

#endif