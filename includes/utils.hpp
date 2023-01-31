/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 09:53:33 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/31 17:48:08 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP
# include <vector>
# include "ircserv.hpp"

namespace irc{
    std::vector<str>    ft_split(str msg, str delim);
    str                 ft_current_time(void);
    str                 ft_ms_to_duration(unsigned long ms);
}


#endif