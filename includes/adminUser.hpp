/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adminUser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 08:46:16 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/24 12:18:04 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ADMIN_USER_HPP
# define ADMIN_USER_HPP
# include "ircserv.hpp"
# include "user.hpp"

namespace irc{
    
    class adminUser : public user {            
        public:
        // ----- Constructors -----
            adminUser( void );
        // ----- Destructor -----
            ~adminUser( void );
        // ----- Read Connexion ------
            int read_connection( void );
    };

}

#endif