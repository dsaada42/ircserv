/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 08:21:42 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/23 09:45:14 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "message.hpp"

// ----- Constructor -----
irc::message::message( void ){}

irc::message::message( std::string message ): _message(message){}


// ----- Destructor -----
irc::message::~message( void ){}

// ----- Getters -----
std::string irc::message::get_message( void )  const { return (_message); }
std::string irc::message::get_prefix( void )   const { return (_prefix); }
std::string irc::message::get_cmd( void )      const { return (_cmd); }
std::string irc::message::get_params( void )   const { return (_params); }
std::string irc::message::get_trailing( void ) const { return (_trailing); }
const int & irc::message::to( void )           const { return (_to); }

// ----- Setters -----
void irc::message::set_message(const std::string & message)     { _message = message; }
void irc::message::set_params(const std::string & params)       { _params = params; }
void irc::message::set_prefix(const std::string & prefix)       { _prefix = prefix; }
void irc::message::set_cmd(const std::string & cmd)             { _cmd = cmd; }
void irc::message::set_trailing(const std::string & trailing)   { _trailing = trailing; }
void irc::message::set_to(const int & to)                       { _to = to; }

// ----- Decoder -----
int irc::message::parse_message( void ){
    //all the parsing stuff for said cmd
    return (0);
}

// ----- Encoder -----
std::string irc::message::create_message( void ){
    _message = _prefix + _cmd + _params + _trailing;
    return (_message);
}

// ----- Send -----
int irc::message::send( void ){
    return (write(_to, _message.c_str(), _message.size()));
}