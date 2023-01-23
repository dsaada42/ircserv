/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 10:54:59 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/23 12:39:18 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "user.hpp"

// ----- Constructor default -----
irc::user::user( int fd ): _fd(fd), _remain(0){}
irc::user::user( std::string nickname, std::string username, std::string fullname, bool oper, int fd)
    : _username(username), _nickname(nickname), _fullname(fullname), _oper(oper), _fd(fd), _remain(0){}

// ----- Copy Constructor -----
irc::user::user( const irc::user & x)
    : _username(x.username()), _nickname(x.nickname()), _fullname(x.fullname()), _oper(x.oper()), _fd(x.fd()), _remain(0){}
irc::user::~user( void ){
    while( _messages.size() != 0){
        delete _messages.front();
        _messages.pop();
    }
}

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

// ----- Setters -----
void irc::user::set_username(const std::string & username)  { _username = username; }
void irc::user::set_fullname(const std::string & fullname)  { _fullname = fullname; }
void irc::user::set_nickname(const std::string & nickname)  { _nickname = nickname; }
void irc::user::set_oper    (const bool & oper)             { _oper = oper; }

// ----- Read Connexion -----
int irc::user::read_connection(void)                       { 
    // on lit depuis le fd
    // attention a check le cas ou on recupere 0 -> signifie une deconnexion 
    if (read(_fd, _buff + _remain, sizeof(_buff) - _remain - 1) <= 0){
        return (1);
        //delete / disconnect user if fail on reading socket
    }
    extract_messages();
    return (0);
}

// ----- Extract all available messages -----
void irc::user::extract_messages(void){
    std::string buff = _buff;
    std::string delim = "\r\n";
    std::string::size_type    i;
    
    // tant qu'on trouve un delimiteur, on recupere la commande et passe a la suivante 
    while ((i = buff.find(delim)) != std::string::npos){
        std::cout << "found cmd : " << buff.substr(0, i) << std::endl;
        _messages.push(new message(buff.substr(0, i)));
        buff = buff.substr(i + 2, buff.size());
    }
    // on reinitialise le buffer et laisse la data restante a l'interieur 
    bzero(_buff, BUFF_SIZE);
    std::cout << "current buffer content :\n" << buff << std::endl;
    _remain = buff.size();
    for (i = 0; i < _remain; i++)
        _buff[i] = buff[i];
}