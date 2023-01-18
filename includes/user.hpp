/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 09:19:34 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/18 14:54:06 by dsaada           ###   ########.fr       */
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
            int         _fd;
        
        public:
            user(void);
            user(std::string nickname, std::string username, std::string fullname, bool mode);
            ~user( void );


            // ----- Getters ------
            std::string & get_username( void );
            std::string & get_nickname( void );
            std::string & get_fullname( void );
            bool        & get_oper( void );
            int         & get_fd( void );
            // ----- Setters -----
            void set_username( const std::string & username );
            void set_nickname( const std::string & nickname );
            void set_fullname( const std::string & fullname );
            void set_oper( const bool & oper );
            void set_fd( )
    };

}

#endif