/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 10:54:59 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/24 09:02:17 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "user.hpp"

// ----- Constructor default -----
irc::user::user( int fd ): _fd(fd), _remain(0){
    bzero(_buff, BUFF_SIZE);
}
irc::user::user( str nickname, str username, str fullname, bool oper, int fd)
    : _username(username), _nickname(nickname), _fullname(fullname), _oper(oper), _fd(fd), _remain(0){
    bzero(_buff, BUFF_SIZE);
}

// ----- Copy Constructor -----
irc::user::user( const irc::user & x)
    : _username(x.username()), _nickname(x.nickname()), _fullname(x.fullname()), _oper(x.oper()), _fd(x.fd()), _remain(0){}
// ----- Destructor -----
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
const str & irc::user::username     ( void ) const  { return(_username);}
const str & irc::user::fullname     ( void ) const  { return(_fullname);}
const str & irc::user::nickname     ( void ) const  { return(_nickname);}
const bool        & irc::user::oper ( void ) const  { return(_oper);}
const int         & irc::user::fd   ( void ) const  { return(_fd);}

// ----- Setters -----
void irc::user::set_username(const str & username)  { _username = username; }
void irc::user::set_fullname(const str & fullname)  { _fullname = fullname; }
void irc::user::set_nickname(const str & nickname)  { _nickname = nickname; }
void irc::user::set_oper    (const bool & oper)     { _oper = oper; }

// ----- Read Connexion -----
int irc::user::read_connection(void)                { 
    // on lit depuis le fd
    // attention a check le cas ou on recupere 0 -> signifie une deconnexion 
    if (read(_fd, _buff + _remain, sizeof(_buff) - _remain - 1) <= 0){
        return (FAILURE);
    }
    extract_messages("\r\n");
    return (SUCCESS);
}

// ----- Extract all available messages -----
void irc::user::extract_messages(str delim)              {
    str buff = _buff;
    str::size_type    i;
    
    // tant qu'on trouve un delimiteur, on recupere la commande et passe a la suivante 
    while ((i = buff.find(delim)) != str::npos){
        std::cout << "found cmd : " << buff.substr(0, i) << std::endl;
        _messages.push(new message(buff.substr(0, i)));
        buff = buff.substr(i + delim.size(), buff.size());
    }
    // on reinitialise le buffer et laisse la data restante a l'interieur 
    bzero(_buff, BUFF_SIZE);
    std::cout << "current buffer content :\n" << buff << std::endl;
    _remain = buff.size();
    for (i = 0; i < _remain; i++)
        _buff[i] = buff[i];
}