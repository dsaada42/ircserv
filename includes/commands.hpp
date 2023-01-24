/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 10:14:11 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/24 12:24:36 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_LIST_HPP
# define COMMAND_LIST_HPP
# include "message.hpp"
 
int ft_admin( irc::message * );
int ft_cap( irc::message * );
int ft_error( irc::message * );
int ft_info( irc::message * );
int ft_invite( irc::message * );
int ft_join( irc::message * );
int ft_kick( irc::message * );
int ft_kill( irc::message * );
int ft_list( irc::message * );
int ft_mode(irc::message *);
int ft_names(irc::message *);
int ft_nick(irc::message *);
int ft_notice(irc::message *);
int ft_oper(irc::message *);
int ft_pass(irc::message *);
int ft_part(irc::message *);
int ft_ping(irc::message *);
int ft_pong(irc::message *);
int ft_privmsg(irc::message *);
int ft_quit(irc::message *);
int ft_stats(irc::message *);
int ft_time(irc::message *);
int ft_topic(irc::message *);
int ft_user(irc::message *);
int ft_version(irc::message *);
int ft_who(irc::message *);
int ft_whois(irc::message *);
int ft_whowas(irc::message *);

#endif