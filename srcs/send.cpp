/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 17:07:50 by dsaada            #+#    #+#             */
/*   Updated: 2023/02/07 17:28:03 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "send.hpp"

namespace irc{
    
    namespace err
    {
        irc::message * err_nosuchnick(const str &nickname, int to)									{ return (new irc::message(SERVER_NAME, SSTR(ERR_NOSUCHNICK), 		nickname, 		"No such nick/channel", to)); } 
        irc::message * err_nosuchserver(const str &srvname, int to)								    { return (new irc::message(SERVER_NAME, SSTR(ERR_NOSUCHSERVER), 	srvname, 	    "No such server", to));} 
        irc::message * err_nosuchchannel(const str &channelname, int to)							{ return (new irc::message(SERVER_NAME, SSTR(ERR_NOSUCHCHANNEL), 	channelname, 	"No such channel", to));} 
        irc::message * err_cannotsendtochan(const str &channelname, int to)							{ return (new irc::message(SERVER_NAME, SSTR(ERR_CANNOTSENDTOCHAN), channelname, 	"Cannot send to channel", to));} 
        irc::message * err_toomanychannels(const str &channelname, int to)							{ return (new irc::message(SERVER_NAME, SSTR(ERR_TOOMANYCHANNELS), 	channelname, 	"You have joined too many channels", to));}
        irc::message * err_wasnosuchnick(const str &sender, const str &nickname, int to)			{ return (new irc::message(SERVER_NAME, SSTR(ERR_WASNOSUCHNICK), 	sender + " " + nickname, 		"There was no such nickname", to));}
        irc::message * err_toomanytargets(const str &target, int to)								{ return (new irc::message(SERVER_NAME, SSTR(ERR_TOOMANYTARGETS), 	target, 		"Duplicate recipients. No message delivered", to));}
        irc::message * err_noorigin(int to)														    { return (new irc::message(SERVER_NAME, SSTR(ERR_NOORIGIN), 		"", 			"No origin specified", to));}
        irc::message * err_norecipient(const str &command, int to)									{ return (new irc::message(SERVER_NAME, SSTR(ERR_NORECIPIENT), 		"", 			"No recipient given " + command, to));}
        irc::message * err_notexttosend(int to) 													{ return (new irc::message(SERVER_NAME, SSTR(ERR_NOTEXTTOSEND), 	"", 			"No text to send", to));}
        irc::message * err_notoplevel(const str &mask, int to)										{ return (new irc::message(SERVER_NAME, SSTR(ERR_NOTOPLEVEL), 		mask, 			"No top level domain specified", to));}
        irc::message * err_wildtoplevel(const str &mask, int to)									{ return (new irc::message(SERVER_NAME, SSTR(ERR_WILDTOPLEVEL), 	mask, 			"Wildcard in top level domain", to));}
        irc::message * err_unknowncommand(const str &command, int to)								{ return (new irc::message(SERVER_NAME, SSTR(ERR_UNKNOWNCOMMAND), 	command, 		"Unknown command", to));}
        irc::message * err_nomotd(int to)   														{ return (new irc::message(SERVER_NAME, SSTR(ERR_NOMOTD), 			"", 			"MOTD file is missing", to));}
        irc::message * err_noadmininfo(int to)  													{ return (new irc::message(SERVER_NAME, SSTR(ERR_NOADMINFO), 		"", 			"No administrative info available", to));}
        irc::message * err_nonicknamegiven(int to)	    											{ return (new irc::message(SERVER_NAME, SSTR(ERR_NONICKNAMEGIVEN), 	"", 			"No nickame given", to));}
        irc::message * err_erroneusnickname(const str &nickname, int to)							{ return (new irc::message(SERVER_NAME, SSTR(ERR_ERRONEUSNICKNAME), nickname, 		"Erroneus nickname", to));}
        irc::message * err_nicknameinuse(const str &nickname, int to)								{ return (new irc::message(SERVER_NAME, SSTR(ERR_NICKNAMEINUSE), 	nickname,	 	nickname, to));}
        irc::message * err_usernotinchannel(const str &nickname, const str &target, const str &channel, int to)		{ return (new irc::message(SERVER_NAME, SSTR(ERR_USERNOTINCHANNEL), nickname + ' ' + channel, "User " + target + " is not in that channel", to));}
        irc::message * err_notonchannel(const str &channel, int to)									{ return (new irc::message(SERVER_NAME, SSTR(ERR_NOTONCHANNEL), 	channel, 		"You're not on that channel", to));}
        irc::message * err_useronchannel(const str &nickname, const str &channel, int to)			{ return (new irc::message(SERVER_NAME, SSTR(ERR_USERONCHANNEL), 	nickname + ' ' + channel, "is already on channel", to));}
        irc::message * err_summondisabled(int to)													{ return (new irc::message(SERVER_NAME, SSTR(ERR_SUMMONDISABLED), 	"", 			"Summon has been disabled", to));}
        irc::message * err_usersdisabled(int to) 													{ return (new irc::message(SERVER_NAME, SSTR(ERR_USERSDISABLED), 	"", 			"USERS has been disabled", to));}
        irc::message * err_notregistered(int to) 													{ return (new irc::message(SERVER_NAME, SSTR(ERR_NOTREGISTERED), 	"", 			"You have not registered", to));}
        irc::message * err_needmoreparams(const str &command, int to)								{ return (new irc::message(SERVER_NAME, SSTR(ERR_NEEDMOREPARAMS), 	command, 		"Not enough parameters", to));}
        irc::message * err_alreadyregistred(int to)					    							{ return (new irc::message(SERVER_NAME, SSTR(ERR_ALREADYREGISTRED),	"", 			"Unauthorized command (already registered)", to));}
        irc::message * err_nopermforhost(int to)													{ return (new irc::message(SERVER_NAME, SSTR(ERR_NOPERMFORHOST), 	"", 			"Your host is not among the privileged", to));}
        irc::message * err_passwdmissmatch(int to) 						    						{ return (new irc::message(SERVER_NAME, SSTR(ERR_PASSWDMISSMATCH), 	"", 			"Password incorrect", to));}
        irc::message * err_yourebannedcreep(int to)							    					{ return (new irc::message(SERVER_NAME, SSTR(ERR_YOUREBANNEDCREEP), "", 			"You are banned from this server", to));}
        irc::message * err_keyset(const str &channel, int to)										{ return (new irc::message(SERVER_NAME, SSTR(ERR_KEYSET), 			channel, 		"Channel key already set", to));}
        irc::message * err_channelisfull(const str &channel, int to)								{ return (new irc::message(SERVER_NAME, SSTR(ERR_CHANNELISFULL), 	channel, 		"Channel is full", to));}
        irc::message * err_unknownmode(const str &nickname, const str &c, int to)				    { return (new irc::message(SERVER_NAME, SSTR(ERR_UNKNOWNMODE), 		nickname,       c + " is unknown mode char to me", to));}
        irc::message * err_inviteonlychan(const str &nickname, const str &channel, int to)			{ return (new irc::message(SERVER_NAME, SSTR(ERR_INVITEONLYCHAN), 	nickname + " " + channel, 		"Cannot join channel (+i)", to));}
        irc::message * err_bannedfromchan(const str &channel, int to)								{ return (new irc::message(SERVER_NAME, SSTR(ERR_BANNEDFROMCHAN), 	channel, 		"Cannot join channel (+b)", to));}
        irc::message * err_badchannelkey(const str &nickname, const str &channel, int to)			{ return (new irc::message(SERVER_NAME, SSTR(ERR_BADCHANNELKEY),    nickname + ' ' + channel, 		"Cannot join channel (+k)", to));}
        irc::message * err_noprivileges(int to)						    							{ return (new irc::message(SERVER_NAME, SSTR(ERR_NOPRIVILEGES),     "", 			"Permission Denied- You're not an IRC operator", to));}
        irc::message * err_chanoprivsneeded(const str &channel, int to)								{ return (new irc::message(SERVER_NAME, SSTR(ERR_CHANOPRIVSNEEDED), channel, 		"You're not channel operator", to));}
        irc::message * err_cantkillserver(int to) 						    						{ return (new irc::message(SERVER_NAME, SSTR(ERR_CANTKILLSERVER), 	"", 			"You can't kill a server", to));}
        irc::message * err_nooperhost(int to)														{ return (new irc::message(SERVER_NAME, SSTR(ERR_NOOPERHOST), 		"", 			"You can't be operator", to));}
        irc::message * err_umodeunknownflag(int to)							    					{ return (new irc::message(SERVER_NAME, SSTR(ERR_UMODEUNKNOWNFLAG), "", 			"Unknown MODE flag", to));}
        irc::message * err_usersdontmatch(int to)													{ return (new irc::message(SERVER_NAME, SSTR(ERR_USERSDONTMATCH), 	"", 			"Can't change mode for other users", to));}
    }

    namespace rpl
    {           
        irc::message * rpl_welcome(const str &nick, int to)									        { return (new irc::message(SERVER_NAME, "001", 			            nick, 			"Welcome to the Internet Relay Network " + nick, to));}
        irc::message * rpl_yourhost(const str &nick, const str &srvname, const str &version, int to){ return (new irc::message(SERVER_NAME, "002", 		                nick, 			"Your host is " + srvname + ", running version " + version, to));}
        irc::message * rpl_created(const str &nick, const str &date, int to)						{ return (new irc::message(SERVER_NAME, "003", 			            nick, 			"This server was created " + date, to));}
        irc::message * rpl_myinfo(const str &nick, const str &srvname, const str &version, const str &umodes, const str &cmodes, int to)            
                                                                                                    { return (new irc::message(SERVER_NAME, "004", 			            nick, 			srvname + ' ' + version + " [user modes: " + umodes + "] [channel modes: " + cmodes + "]", to));}
        irc::message * rpl_bounce(const str &nick, const str &srvname, const str &port, int to)	    { return (new irc::message(SERVER_NAME, "005", 			            nick, 			"Try server " + srvname + ", port " + port, to));}
        irc::message * rpl_statslinkinfo(const str &nickname, const str &stats, int to)             { return (new irc::message(SERVER_NAME, SSTR(RPL_STATSLINKINFO),    nickname, stats, to));}
        irc::message * rpl_statscommands(const str &nickname, const str &command, int to)           { return (new irc::message(SERVER_NAME, SSTR(RPL_STATSCOMMANDS),    nickname, command, to));}
        irc::message * rpl_endofstats(const str &nickname, const str &letter, int to)				{ return (new irc::message(SERVER_NAME, SSTR(RPL_ENDOFSTATS), 		nickname + " " + letter, 		"End of STATS report", to));}
        irc::message * rpl_umodeis(const str &nickname, const str &modes, int to)					{ return (new irc::message(SERVER_NAME, SSTR(RPL_UMODEIS), 		    nickname, modes, to));}
        irc::message * rpl_statsuptime(const str &nickname, const str &stats, int to)               { return (new irc::message(SERVER_NAME, SSTR(RPL_STATSUPTIME),      nickname,       "Server up for " + stats, to));}
        irc::message * rpl_liststart(int to)                                                        { return (new irc::message(SERVER_NAME, SSTR(RPL_LISTSTART), "", "List START", to));}
        irc::message * rpl_list(const str &nick, const str &channel, const str &topic, int to)		{ return (new irc::message(SERVER_NAME, SSTR(RPL_LIST), 			nick + " " +  channel + ' ' + topic, "",  to));} // because SSTR not working here
        irc::message * rpl_listend(int to)											    			{ return (new irc::message(SERVER_NAME, SSTR(RPL_LISTEND), 			"", 			"End of LIST", to));}
        irc::message * rpl_channelmodeis(const str &nickname, const str &channel, const str &modes, int to)	{ return (new irc::message(SERVER_NAME, SSTR(RPL_CHANNELMODEIS), 	nickname + " " + channel + " +" + modes, "", to));}
        irc::message * rpl_notopic(const str &nickname, const str &channel, int to)					{ return (new irc::message(SERVER_NAME, SSTR(RPL_NOTOPIC), 			nickname + ' ' + channel, 		"No topic set", to));}
        irc::message * rpl_topic(const str& nickname, const str &channel, const str &topic, int to)	{ return (new irc::message(SERVER_NAME, SSTR(RPL_TOPIC), 			nickname + ' ' + channel, 		topic, to));}
        irc::message * rpl_inviting(const str &channel, const str &nickname, const str &target, int to){ return (new irc::message(SERVER_NAME, SSTR(RPL_INVITING), 		nickname + ' ' + target + ' ' + channel, "", to));}
        irc::message * rpl_version(const str &nickname, const str &version, const str debuglevel, const str &server, const str &comments, int to)	
                                                                                                    { return (new irc::message(SERVER_NAME, SSTR(RPL_VERSION), 			nickname,  version + '.' + debuglevel + ' ' + server + ' ' + comments, to));}
        irc::message * rpl_namreply(const str &nickname, const str &channel_name, const str &names, int to)	{ return (new irc::message(SERVER_NAME, SSTR(RPL_NAMREPLY), 	nickname + " = " + channel_name , names, to));}
        irc::message * rpl_endofnames(const str &name, const str &channel_name, int to)				{ return (new irc::message(SERVER_NAME, SSTR(RPL_ENDOFNAMES), 		name + " " + channel_name, 			"End of NAMES list", to));}
        irc::message * rpl_banlist(const str &channel, const str &banmask, int to)					{ return (new irc::message(SERVER_NAME, SSTR(RPL_BANLIST), 			channel + ' ' + banmask, "", to));}
        irc::message * rpl_endofbanlist(const str &channel, int to)									{ return (new irc::message(SERVER_NAME, SSTR(RPL_ENDOFBANLIST), 	channel, 		"End of channel ban list", to));}
        irc::message * rpl_motd(const str &text, int to)											{ return (new irc::message(SERVER_NAME, SSTR(RPL_MOTD), 			"", 			"- " + text, to));}
        irc::message * rpl_motdstart(const str &srvname, int to)									{ return (new irc::message(SERVER_NAME, SSTR(RPL_MOTDSTART), 		"", 			"- " + srvname + " Message of the day - ", to));}
        irc::message * rpl_endofmotd(int to)														{ return (new irc::message(SERVER_NAME, SSTR(RPL_ENDOFMOTD), 		"", 			"End of MOTD command", to));}
        irc::message * rpl_info(const str &info, int to)											{ return (new irc::message(SERVER_NAME, SSTR(RPL_INFO), 			"", 			info, to));}
        irc::message * rpl_endofinfo(int to)														{ return (new irc::message(SERVER_NAME, SSTR(RPL_ENDOFINFO), 		"", 			"End of INFO list", to));}
        irc::message * rpl_youreoper(const str &nickname, int to)									{ return (new irc::message(SERVER_NAME, SSTR(RPL_YOUREOPER), 		nickname, 		"You are now an IRC operator", to));}
        irc::message * rpl_time(const str &srvname, int to)										    { return (new irc::message(SERVER_NAME, SSTR(RPL_TIME), 			srvname, 	    ft_current_time(), to));} 
        irc::message * rpl_whoisuser(const str &sender, const str &nickname, const str &username, const str &host, const str &fullname, int to)	
                                                                                                    { return (new irc::message(SERVER_NAME, SSTR(RPL_WHOISUSER), 		sender + " " + nickname + " ~" + username + " " + host + " *", fullname, to));}
        irc::message * rpl_whoisserver(const str &sender, const str &nick, const str &srvname, const str shortinfo, int to)
                                                                                                    { return (new irc::message(SERVER_NAME, SSTR(RPL_WHOISSERVER), 		sender + " " + nick + " " + srvname, shortinfo, to));}
        irc::message * rpl_whoisoperator(const str &nick, int to)									{ return (new irc::message(SERVER_NAME, SSTR(RPL_WHOISOPERATOR), 	nick, 			"is an IRC operator", to));}
        irc::message * rpl_whoisidle(const str &sender, const str &nickname, const str &idle, const str &timestamp, int to)	{ return (new irc::message(SERVER_NAME, SSTR(RPL_WHOISIDLE), 		sender + " " + nickname + " " + idle + " " + timestamp, "seconds idle, singon time", to));}
        irc::message * rpl_whoischannels(const str &sender, const str &nickname, const str &chans, int to)
                                                                                                    { return (new irc::message(SERVER_NAME, SSTR(RPL_WHOISCHANNELS), 	sender + " " + nickname, chans, to));}
        irc::message * rpl_endofwhois(const str &sender, const str &nicks, int to)					{ return (new irc::message(SERVER_NAME, SSTR(RPL_ENDOFWHOIS), 		sender + " " + nicks, "End of /WHOIS list", to));}
        irc::message * rpl_whoreply(const str &sender, const str &msg, const str &trailing, int to)	{ return (new irc::message(SERVER_NAME, SSTR(RPL_WHOREPLY), 		sender + " " + msg, trailing , to));}
        irc::message * rpl_endofwho(const str &nickname, const str &target, int to)					{ return (new irc::message(SERVER_NAME, SSTR(RPL_ENDOFWHO), 		nickname + " "  +  target, 		"End of /WHO list", to));}
        irc::message * rpl_whowasuser(const str &sender, const str &nick, const str &user, const str &host, const str &realname, int to)	
                                                                                                    { return (new irc::message(SERVER_NAME, SSTR(RPL_WHOWASUSER), 		sender + " " + nick + " ~" + user + " " + host + " *", realname, to));}
        irc::message * rpl_endofwhowas(const str &sender, const str &nick, int to)					{ return (new irc::message(SERVER_NAME, SSTR(RPL_ENDOFWHOWAS), 		sender + " " + nick, 			"End of WHOWAS", to));}
        irc::message * rpl_userhostreply(const str &msg, int to)									{ return (new irc::message(SERVER_NAME, SSTR(RPL_USERHOST), 		"", 			msg, to));}
    }

    namespace cmd
    {
        irc::message * cmd_ping(const str &nick, int to)                                            { return (new irc::message(SERVER_NAME, "PING", nick, SERVER_NAME, to));}
        irc::message * cmd_mode(const str &target, const str &param, int to)                        { return (new irc::message(SERVER_NAME, "MODE", target, param, to));}
        irc::message * cmd_nick(const str &user_prefix, const str &nick, int to)                    { return (new irc::message(user_prefix, "NICK", "", nick, to));}   
        irc::message * cmd_quit(const str &user_prefix, const str &msg, int to)                     { return (new irc::message(user_prefix, "QUIT", "", msg, to));}    
        irc::message * cmd_privmsg(const str &user_prefix, const str &target, const str &msg, int to){ return (new irc::message(user_prefix, "PRIVMSG", target, msg, to));}
        irc::message * cmd_notice(const str &user_prefix, const str &target, const str &msg, int to){ return (new irc::message(user_prefix, "NOTICE", target, msg, to));}
	    irc::message * cmd_join(const str &user_prefix, const str &channel_name, int to)	        { return (new irc::message(user_prefix, "JOIN", "", channel_name, to));}
        irc::message * cmd_invite(const str &user_prefix, const str &target, const str &channel_name, int to) {return (new irc::message(user_prefix, "INVITE", target, channel_name, to));}
        irc::message * cmd_part(const str &user_prefix, const str &channel_name, const str &msg, int to){ return (new irc::message(user_prefix, "PART", channel_name, msg, to));}
        irc::message * cmd_mode_channel(const str &user_prefix, const str &channel_name, const str &mode, int to){ return (new irc::message(user_prefix, "MODE", channel_name + " " + mode, "", to));}
        irc::message * cmd_pong(const str& param, int to)                                           { return (new irc::message(SERVER_NAME, "PONG", param, "", to));}
    } 

}
