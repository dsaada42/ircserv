/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adminUser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 08:47:52 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/25 09:08:33 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "adminUser.hpp"

// ----- Constructor default -----
irc::adminUser::adminUser( void ): user( "adminick", "useradmin", "Admin D. Ballec", true, 0){}

// ----- Destructor -----
irc::adminUser::~adminUser( void ){}

// ----- Read Connexion -----
int irc::adminUser::read_connection(void){ 
    str msg;
    // on lit depuis le fd
    if (read(_fd, _buff + _remain, sizeof(_buff) - _remain - 1) < 0){
        return (FAILURE);
    }

    return (SUCCESS);
}