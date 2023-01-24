/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   admin.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 08:46:16 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/24 08:58:49 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ADMIN_HPP
# define ADMIN_HPP
# include "ircserv.hpp"
# include "user.hpp"

namespace irc{
    
    class admin : public user {            
        public:
        // ----- Constructors -----
            admin( void );
        // ----- Destructor -----
            ~admin( void );
        // ----- Read Connexion ------
            int read_connection( void );
    };

}

#endif