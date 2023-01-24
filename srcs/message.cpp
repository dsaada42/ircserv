/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 08:21:42 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/24 10:37:46 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "message.hpp"

// ----- Constructor -----
irc::message::message( void ){}

irc::message::message( str message ): _message(message){}

// ----- Destructor -----
irc::message::~message( void ){}

// ----- Getters -----
str irc::message::get_message( void )  const { return (_message); }
str irc::message::get_prefix( void )   const { return (_prefix); }
str irc::message::get_cmd( void )      const { return (_cmd); }
str irc::message::get_params( void )   const { return (_params); }
str irc::message::get_trailing( void ) const { return (_trailing); }
const int & irc::message::to( void )   const { return (_to); }

// ----- Setters -----
void irc::message::set_message(const str & message)     { _message = message; }
void irc::message::set_params(const str & params)       { _params = params; }
void irc::message::set_prefix(const str & prefix)       { _prefix = prefix; }
void irc::message::set_cmd(const str & cmd)             { _cmd = cmd; }
void irc::message::set_trailing(const str & trailing)   { _trailing = trailing; }
void irc::message::set_to(const int & to)               { _to = to; }

// ----- Decoder -----
int irc::message::parse_message( void ){
    //all the parsing stuff for said cmd
    return(1);
}

// ----- Encoder -----
str irc::message::create_message( void ){
    _message = _prefix + _cmd + _params + _trailing;
    return (_message);
}

// ----- Send -----
int irc::message::send( void ){
    return (write(_to, _message.c_str(), _message.size()));
}