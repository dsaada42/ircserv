/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 09:52:47 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/21 12:03:12 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP
# include "ircserv.hpp"

namespace irc{
    
    class message{
        
        public:
            typedef std::string     str;
            
        private:
            str _message;
            str _params;
            str _prefix;
            str _cmd;
            str _trailing;

        public:
        // ----- Constructors -----
            message( void );
            message( str message );
            
        // ----- Destructor -----
            ~message( void );

        // ----- Getters -----
            str get_message( void ) const;
            str get_prefix( void ) const;
            str get_params( void ) const;
            str get_cmd( void ) const;
            str get_trailing( void ) const;
            
        // ----- Setters -----
            void set_message(const str & message);
            void set_prefix(const str & prefix);
            void set_params(const str & params);
            void set_cmd(const str & cmd);
            void set_trailing(const str & trailing);

        // ----- Decoder -----
            int parse_message( void );

        // ----- Encoder -----
            str create_message( void );
    };

}

#endif