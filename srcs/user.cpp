/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 10:54:59 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/19 11:24:26 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "user.hpp"

// ----- Constructor default -----
irc::user::user( void ) : _username("default"), _nickname("default"), _fullname("default"), _oper(0){}

irc::user::user( std::string nickname, std::string username, std::string fullname, bool oper )
    : _username(username), _nickname(nickname), _fullname(fullname), _oper(oper){}
// ----- Copy Constructor -----
irc::user::user( const irc::user & x) 
    : _username(x.get_username()), _nickname(x.get_nickname()), _fullname(x.get_fullname()), _oper(x.get_oper()), _fd(x.get_fd()){}
irc::user::~user( void ){}

// ----- Operator= -----
irc::user & irc::user::operator= (const irc::user & x){
    _username = x.get_username();
    _nickname = x.get_nickname();
    _fullname = x.get_fullname();
    _oper = x.get_oper();
    _fd = x.get_fd();
    return (*this);
}

// ----- Getters -----
const std::string & irc::user::get_username ( void ) const        { return(_username);}
const std::string & irc::user::get_fullname ( void ) const        { return(_fullname);}
const std::string & irc::user::get_nickname ( void ) const        { return(_nickname);}
const bool        & irc::user::get_oper     ( void ) const        { return(_oper);}
const int         & irc::user::get_fd       ( void ) const        { return(_fd);}
// ----- Setters -----
void irc::user::set_username(const std::string & username)  { _username = username; }
void irc::user::set_fullname(const std::string & fullname)  { _fullname = fullname; }
void irc::user::set_nickname(const std::string & nickname)  { _nickname = nickname; }
void irc::user::set_oper    (const bool & oper)             { _oper = oper; }
void irc::user::set_fd      (const int & fd)                { _fd = fd; }
