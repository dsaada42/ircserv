/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 10:54:59 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/17 11:44:21 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "user.hpp"

// ----- Constructor -----
irc::user::user( void ) : _username("default"), _nickname("default"), _fullname("default"), _oper(0){}

irc::user::user( std::string nickname, std::string username, std::string fullname, bool oper )
    : _username(username), _nickname(nickname), _fullname(fullname), _oper(oper){}

irc::user::~user( void ){}


// ----- Getters -----
std::string & irc::user::get_username ( void )        { return(_username);}
std::string & irc::user::get_fullname ( void )        { return(_fullname);}
std::string & irc::user::get_nickname ( void )        { return(_nickname);}
bool        & irc::user::get_oper     ( void )        { return(_oper);}

// ----- Setters -----
void irc::user::set_username(const std::string & username)  { _username = username; }
void irc::user::set_fullname(const std::string & fullname)  { _fullname = fullname; }
void irc::user::set_nickname(const std::string & nickname)  { _nickname = nickname; }
void irc::user::set_oper    (const bool & oper)             { _oper = oper; }

