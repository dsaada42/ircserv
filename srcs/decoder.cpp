/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decoder.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 11:07:32 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/21 13:02:19 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "decoder.hpp"

// ----- Constructor -----
irc::decoder::decoder(int socket_fd) : _fd(socket_fd){}

// ----- Destructor -----
irc::decoder::~decoder(void){}

// ----- Extract all available messages -----
int irc::decoder::extract_messages(void){
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
    for (i = 0; i < buff.size(); i++)
        _buff[i] = buff[i];
    return (0);
}

// ----- Run -----
int irc::decoder::run(void){
    // on lit depuis le fd
    // attention a check le cas ou on recupere 0 -> signifie une deconnexion 
    read(_fd, _buff, sizeof(_buff) - 1);
    extract_messages();
    return (0);
}
