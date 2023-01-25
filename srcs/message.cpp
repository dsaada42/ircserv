/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 08:21:42 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/25 08:44:49 by dsaada           ###   ########.fr       */
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
    str::size_type  pos;
    str             msg = _message;
    
    //if prefix detected
    if (msg.at(0) == ':'){
        std::cout << "detected prefix" << std::endl;
        pos = msg.find(" ");
        if (pos == str::npos){
            std::cout << "no cmd detected, failed parsing message" << std::endl;  
            return (FAILURE);
        } //cas ou rien derriere le prefixe -> cmd not valid
        _prefix = msg.substr( 0, pos );
        msg = msg.substr(pos + 1, msg.size());
    }
    pos = msg.find(" ");
    _cmd = msg.substr( 0 , pos ); 
    if ( pos == str::npos ) //cas ou uniquement commande sans arguments 
        return (SUCCESS);
    msg = msg.substr( pos + 1, msg.size());
    pos = msg.find(":");
    if (pos == str::npos){ // no trailing found, the rest is args
        _params = msg;
    }
    else{ //found trailing 
        _params = msg.substr(0, pos);
        _trailing = msg.substr(pos + 1, msg.size());
    }
    return(SUCCESS);
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