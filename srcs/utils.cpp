/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 09:59:03 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/31 18:28:52 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

std::vector<str> irc::ft_split(str msg, str delim){
    std::vector<str>     res;
    str::size_type  found;
        
    while ( (found = msg.find(delim)) != str::npos){
        if (found != 0)
            res.push_back(msg.substr(0, found));
        msg = msg.substr(found + delim.size(), msg.size());
    }
    if (msg.size() != 0)
        res.push_back(msg);

    return (res);
}

str irc::ft_current_time(){
    time_t now = time(0);

    str date = ctime(&now);
    return(date);
}

str irc::ft_ms_to_duration(unsigned long ms){
    unsigned long min;
    unsigned long sec;
    unsigned long hour;

    hour = (ms / 1000) / 3600;
    min = (ms / 1000) % 3600 / 60 ;
    sec = (ms / 1000) % 60;
    return ( SSTR(hour) + " hours " + SSTR(min) + " minutes " + SSTR(sec) + " seconds");
}
