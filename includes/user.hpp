/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 09:19:34 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/19 11:27:46 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP
# include "ircserv.hpp"

namespace irc{
            
    class user{
        private:
            std::string _username;
            std::string _nickname;
            std::string _fullname;
            bool        _oper;
            // char        _buffer[BUFF_SIZE];
            int         _fd;
        
        public:
        // ----- Constructors -----
            user(void);
            user(std::string nickname, std::string username, std::string fullname, bool mode);
            user(const user & x);
        // ----- Destructor -----
            ~user( void );
        // ----- Operator= -----
            user& operator= (const user & x);


        // ----- Getters ------
            const std::string & get_username ( void ) const;
            const std::string & get_nickname ( void ) const;
            const std::string & get_fullname ( void ) const;
            const bool        & get_oper     ( void ) const;
            const int         & get_fd       ( void ) const;
        // ----- Setters -----
            void set_username( const std::string & username );
            void set_nickname( const std::string & nickname );
            void set_fullname( const std::string & fullname );
            void set_oper    ( const bool & oper );
            void set_fd      ( const int & fd );
    };

}

#endif