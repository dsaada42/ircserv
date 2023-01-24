/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   admin.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 08:47:52 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/24 10:19:40 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "admin.hpp"

// ----- Constructor default -----
irc::admin::admin( void ): user( "adminick", "useradmin", "Admin D. Ballec", true, 0){}

// ----- Destructor -----
irc::admin::~admin( void ){}

// ----- Read Connexion -----
int irc::admin::read_connection(void){ 
    str msg;
    // on lit depuis le fd
    if (read(_fd, _buff + _remain, sizeof(_buff) - _remain - 1) < 0){
        return (FAILURE);
    }
    extract_messages("\n");
    
    while (_messages.size()){
        msg = _messages.front()->get_message();
        if (msg == "exit")
            throw (exitException());
        delete _messages.front();
        _messages.pop();
    }
    
    return (SUCCESS);
}