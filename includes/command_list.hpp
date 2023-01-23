/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_list.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 10:14:11 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/23 10:22:26 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

struct s_cmd {
    std::string name;
    int (*func)(irc::message*);
}       t_cmd;

t_cmd cmd_list[] = {
    { "ADMIN", admin },
    { "CAP", cap },
    { "ERROR", error },
    { "INFO", info },
    { "INVITE", invite },
    { "JOIN", join },
    { "KICK", kick },
    { "KILL", kill },
    { "LIST", list },
    { "MODE", mode },
    { "NAMES", names },
    { "NICK", nick },
    { "NOTICE", notice },
    { "OPER", oper },
    { "PASS", pass },
    { "PART", part },
    { "PING", ping },
    { "PONG", pong },
    { "PRIVMSG", privmsg },
    { "QUIT", quit },
    { "STATS", stats },
    { "TIME", time },
    { "TOPIC", topic },
    { "USER", user },
    { "VERSION", version },
    { "WHO", who },
    { "WHOIS", whois },
    { "WHOWAS", whowas }
};