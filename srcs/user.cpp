/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 10:54:59 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/21 12:10:26 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "user.hpp"

// ----- Constructor default -----
irc::user::user( int fd ):_decoder(fd), _fd(fd){}
irc::user::user( std::string nickname, std::string username, std::string fullname, bool oper, int fd)
    : _username(username), _nickname(nickname), _fullname(fullname), _decoder(fd), _oper(oper), _fd(fd){}

// ----- Copy Constructor -----
irc::user::user( const irc::user & x)
    : _username(x.username()), _nickname(x.nickname()), _fullname(x.fullname()), _decoder(x.decoder()), _oper(x.oper()), _fd(x.fd()){}
irc::user::~user( void ){}

// ----- Operator= -----
irc::user & irc::user::operator= (const irc::user & x){
    (void)x;
    return (*this);
}

// ----- Getters -----
const std::string & irc::user::username     ( void ) const  { return(_username);}
const std::string & irc::user::fullname     ( void ) const  { return(_fullname);}
const std::string & irc::user::nickname     ( void ) const  { return(_nickname);}
const bool        & irc::user::oper         ( void ) const  { return(_oper);}
const int         & irc::user::fd           ( void ) const  { return(_fd);}
const irc::decoder & irc::user::decoder     ( void ) const  { return(_decoder);}

// ----- Setters -----
void irc::user::set_username(const std::string & username)  { _username = username; }
void irc::user::set_fullname(const std::string & fullname)  { _fullname = fullname; }
void irc::user::set_nickname(const std::string & nickname)  { _nickname = nickname; }
void irc::user::set_oper    (const bool & oper)             { _oper = oper; }

// ----- Read Connexion -----
void irc::user::read_connection(void)                       { _decoder.run();}