/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 08:21:42 by dsaada            #+#    #+#             */
/*   Updated: 2023/02/07 09:03:38 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "message.hpp"

// ----- Constructor -----
irc::message::message( void ){}

irc::message::message( str message, int fd ): _message(message), _fd(fd){}

irc::message::message( str prefix, str cmd, str params, str trailing, int fd )
    : _params(params), _prefix(prefix), _cmd(cmd), _trailing(trailing), _fd(fd){
        create_message();
    }
// ----- Destructor -----
irc::message::~message( void ){}

// ----- Getters -----
str irc::message::get_message( void )  const { return (_message); }
str irc::message::get_prefix( void )   const { return (_prefix); }
str irc::message::get_cmd( void )      const { return (_cmd); }
str irc::message::get_params( void )   const { return (_params); }
str irc::message::get_trailing( void ) const { return (_trailing); }
const int & irc::message::get_fd( void )   const { return (_fd); }

// ----- Setters -----
void irc::message::set_message(const str & message)     { _message = message; }
void irc::message::set_params(const str & params)       { _params = params; }
void irc::message::set_prefix(const str & prefix)       { _prefix = prefix; }
void irc::message::set_cmd(const str & cmd)             { _cmd = cmd; }
void irc::message::set_trailing(const str & trailing)   { _trailing = trailing; }
void irc::message::set_fd(const int & fd)               { _fd = fd; }

// ----- Decoder -----
int irc::message::parse_message( void ){
    str::size_type  pos;
    str             msg = _message;
    
    if (msg.size() == 0)
        return(FAILURE);
    if (msg.at(0) == ':'){
        pos = msg.find(" ");
        if (pos == str::npos){
            std::cout << "no cmd detected, failed parsing message" << std::endl;  
            return (FAILURE);
        }
        _prefix = msg.substr( 1, pos + 1 );
        msg = msg.substr(pos + 1, msg.size());
    }
    pos = msg.find(" ");
    _cmd = msg.substr( 0 , pos ); 
    if ( pos == str::npos ) 
        return (SUCCESS);
    msg = msg.substr( pos + 1, msg.size());
    pos = msg.find(":");
    if (pos == str::npos){
        _params = msg;
    }
    else{ //found trailing 
        _params = msg.substr(0, pos - 1);
        _trailing = msg.substr(pos + 1, msg.size());
    }
    return(SUCCESS);
}

// ----- Encoder -----
str irc::message::create_message( void ){
    if (_prefix.size() > 0)
        _message = ":" + _prefix + " ";
    _message += _cmd;
    if (_params.size() > 0)
        _message += " " + _params;
    if (_trailing.size() > 0)
        _message += " :" + _trailing;
    _message += "\r\n";
    return (_message);
}

// ----- Send -----
int irc::message::send( void ){
    if (write(_fd, _message.c_str(), _message.size()) < 0)
        return (FAILURE);
    return (SUCCESS);
}

void irc::message::print( void ){
    std::cout << "========> Message <========= " << _message << std::endl;
    std::cout << "-> Prefix   : " << _prefix << std::endl;
    std::cout << "-> Cmd      : " << _cmd << std::endl;
    std::cout << "-> Params   : " << _params << std::endl;
    std::cout << "-> Trailing : " << _trailing << std::endl;
    std::cout << "-> Fd       : " << _fd << std::endl;
}