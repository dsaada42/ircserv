/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 10:14:11 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/24 11:47:36 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_LIST_HPP
# define COMMAND_LIST_HPP
# include "message.hpp"
 
int admin( irc::message * );
int cap( irc::message * );
int error( irc::message * );
int info( irc::message * );
int invite( irc::message * );
int join( irc::message * );
int kick( irc::message * );
int kill( irc::message * );
int list( irc::message * );
int mode(irc::message *);
int names(irc::message *);
int nick(irc::message *);
int notice(irc::message *);
int oper(irc::message *);
int pass(irc::message *);
int part(irc::message *);
int ping(irc::message *);
int pong(irc::message *);
int privmsg(irc::message *);
int quit(irc::message *);
int stats(irc::message *);
int time(irc::message *);
int topic(irc::message *);
int user(irc::message *);
int version(irc::message *);
int who(irc::message *);
int whois(irc::message *);
int whowas(irc::message *);

struct s_cmd {
    std::string name;
    int (*func)(irc::message*);
}       t_cmd;

s_cmd cmd_list[] = {
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

#endif