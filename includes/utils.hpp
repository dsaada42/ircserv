/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dylan <dylan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 09:53:33 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/29 21:49:33 by dylan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP
# include <vector>
# include "ircserv.hpp"

namespace irc{
    std::vector<str>    ft_split(str msg, str delim);
    str                 ft_current_time(void);
}


#endif