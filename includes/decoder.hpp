/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decoder.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 10:58:48 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/21 11:01:06 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DECODER_HPP
# define DECODER_HPP
# include "ircserv.hpp"
# include "message.hpp"

namespace irc{
    
    class decoder{
        private:
            char                    _buff[BUFF_SIZE];
            int                     _fd;
            std::queue<message*>    _messages;

        public:
        // ----- Constructor -----
            decoder(int socket_fd);
        // ----- Destructor -----
            ~decoder(void);

        // ----- Run -----
            int run(void);
        private:
        // ----- Extract all available messages -----
            int extract_messages(void);
            
        
    };
}

#endif