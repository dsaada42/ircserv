/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 17:07:50 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/31 16:21:17 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "send.hpp"

namespace irc{
    
    namespace err
    {
        irc::message * err_nosuchnick(const str &nickname, int to)									{ return (new irc::message(SERVER_NAME, SSTR(ERR_NOSUCHNICK), 		nickname, 		"No such nick/channel", to)); } 
        irc::message * err_nosuchserver(const str &srvname, int to)								    { return (new irc::message(SERVER_NAME, SSTR(ERR_NOSUCHSERVER), 	    srvname, 	"No such server", to));} 
        irc::message * err_nosuchchannel(const str &channelname, int to)							{ return (new irc::message(SERVER_NAME, SSTR(ERR_NOSUCHCHANNEL), 	    channelname, 	"No such channel", to));} 
        irc::message * err_cannotsendtochan(const str &channelname, int to)							{ return (new irc::message(SERVER_NAME, SSTR(ERR_CANNOTSENDTOCHAN),   channelname, 	"Cannot send to channel", to));} 
        irc::message * err_toomanychannels(const str &channelname, int to)							{ return (new irc::message(SERVER_NAME, SSTR(ERR_TOOMANYCHANNELS), 	channelname, 	"You have joined too many channels", to));}
        irc::message * err_wasnosuchnick(const str &nickname, int to)								{ return (new irc::message(SERVER_NAME, SSTR(ERR_WASNOSUCHNICK), 	    nickname, 		"There was no such nickname", to));}
        irc::message * err_toomanytargets(const str &target, int to)								{ return (new irc::message(SERVER_NAME, SSTR(ERR_TOOMANYTARGETS), 	target, 		"Duplicate recipients. No message delivered", to));}
        irc::message * err_noorigin(int to)														    { return (new irc::message(SERVER_NAME, SSTR(ERR_NOORIGIN), 		    "", 			"No origin specified", to));}
        irc::message * err_norecipient(const str &command, int to)									{ return (new irc::message(SERVER_NAME, SSTR(ERR_NORECIPIENT), 		"", 			"No recipient given " + command, to));}
        irc::message * err_notexttosend(int to) 													{ return (new irc::message(SERVER_NAME, SSTR(ERR_NOTEXTTOSEND), 	    "", 			"No text to send", to));}
        irc::message * err_notoplevel(const str &mask, int to)										{ return (new irc::message(SERVER_NAME, SSTR(ERR_NOTOPLEVEL), 		mask, 			"No top level domain specified", to));}
        irc::message * err_wildtoplevel(const str &mask, int to)									{ return (new irc::message(SERVER_NAME, SSTR(ERR_WILDTOPLEVEL), 	    mask, 			"Wildcard in top level domain", to));}
        irc::message * err_unknowncommand(const str &command, int to)								{ return (new irc::message(SERVER_NAME, SSTR(ERR_UNKNOWNCOMMAND), 	command, 		"Unknown command", to));}
        irc::message * err_nomotd(int to)   														{ return (new irc::message(SERVER_NAME, SSTR(ERR_NOMOTD), 			"", 			"MOTD file is missing", to));}
        irc::message * err_noadmininfo(int to)  													{ return (new irc::message(SERVER_NAME, SSTR(ERR_NOADMINFO), 		"", 			"No administrative info available", to));}
        irc::message * err_nonicknamegiven(int to)	    											{ return (new irc::message(SERVER_NAME, SSTR(ERR_NONICKNAMEGIVEN), 	"", 			"No nickame given", to));}
        irc::message * err_erroneusnickname(const str &nickname, int to)							{ return (new irc::message(SERVER_NAME, SSTR(ERR_ERRONEUSNICKNAME),   nickname, 		"Erroneus nickname", to));}
        irc::message * err_nicknameinuse(const str &nickname, int to)								{ return (new irc::message(SERVER_NAME, SSTR(ERR_NICKNAMEINUSE), 	    nickname,	 	nickname, to));}
        irc::message * err_usernotinchannel(const str &nickname, const str &channel, int to)		{ return (new irc::message(SERVER_NAME, SSTR(ERR_USERNOTINCHANNEL),   nickname + ' ' + channel, "They aren't on that channel", to));}
        irc::message * err_notonchannel(const str &channel, int to)									{ return (new irc::message(SERVER_NAME, SSTR(ERR_NOTONCHANNEL), 	    channel, 		"You're not on that channel", to));}
        irc::message * err_useronchannel(const str &nickname, const str &channel, int to)			{ return (new irc::message(SERVER_NAME, SSTR(ERR_USERONCHANNEL), 	    nickname + ' ' + channel, "is already on channel", to));}
        irc::message * err_summondisabled(int to)													{ return (new irc::message(SERVER_NAME, SSTR(ERR_SUMMONDISABLED), 	"", 			"Summon has been disabled", to));}
        irc::message * err_usersdisabled(int to) 													{ return (new irc::message(SERVER_NAME, SSTR(ERR_USERSDISABLED), 	    "", 			"USERS has been disabled", to));}
        irc::message * err_notregistered(int to) 													{ return (new irc::message(SERVER_NAME, SSTR(ERR_NOTREGISTERED), 	    "", 			"You have not registered", to));}
        irc::message * err_needmoreparams(const str &command, int to)								{ return (new irc::message(SERVER_NAME, SSTR(ERR_NEEDMOREPARAMS), 	command, 		"Not enough parameters", to));}
        irc::message * err_alreadyregistred(int to)					    							{ return (new irc::message(SERVER_NAME, SSTR(ERR_ALREADYREGISTRED),	"", 			"Unauthorized command (already registered)", to));}
        irc::message * err_nopermforhost(int to)													{ return (new irc::message(SERVER_NAME, SSTR(ERR_NOPERMFORHOST), 	    "", 			"Your host is not among the privileged", to));}
        irc::message * err_passwdmissmatch(int to) 						    						{ return (new irc::message(SERVER_NAME, SSTR(ERR_PASSWDMISSMATCH), 	"", 			"Password incorrect", to));}
        irc::message * err_yourebannedcreep(int to)							    					{ return (new irc::message(SERVER_NAME, SSTR(ERR_YOUREBANNEDCREEP),   "", 			"You are banned from this server", to));}
        irc::message * err_keyset(const str &channel, int to)										{ return (new irc::message(SERVER_NAME, SSTR(ERR_KEYSET), 			channel, 		"Channel key already set", to));}
        irc::message * err_channelisfull(const str &channel, int to)								{ return (new irc::message(SERVER_NAME, SSTR(ERR_CHANNELISFULL), 	    channel, 		"Channel is full", to));}
        irc::message * err_unknownmode(const str &c, int to)										{ return (new irc::message(SERVER_NAME, SSTR(ERR_UNKNOWNMODE), 		c, 				"is unknown mode char to me", to));}
        irc::message * err_inviteonlychan(const str &channel, int to)								{ return (new irc::message(SERVER_NAME, SSTR(ERR_INVITEONLYCHAN), 	channel, 		"Cannot join channel (+i)", to));}
        irc::message * err_bannedfromchan(const str &channel, int to)								{ return (new irc::message(SERVER_NAME, SSTR(ERR_BANNEDFROMCHAN), 	channel, 		"Cannot join channel (+b)", to));}
        irc::message * err_badchannelkey(const str &channel, int to)								{ return (new irc::message(SERVER_NAME, SSTR(ERR_BADCHANNELKEY), 	    channel, 		"Cannot join channel (+k)", to));}
        irc::message * err_noprivileges(int to)						    							{ return (new irc::message(SERVER_NAME, SSTR(ERR_NOPRIVILEGES), 	    "", 			"Permission Denied- You're not an IRC operator", to));}
        irc::message * err_chanoprivsneeded(const str &channel, int to)								{ return (new irc::message(SERVER_NAME, SSTR(ERR_CHANOPRIVSNEEDED), 	channel, 		"You're not channel operator", to));}
        irc::message * err_cantkillserver(int to) 						    						{ return (new irc::message(SERVER_NAME, SSTR(ERR_CANTKILLSERVER), 	"", 			"You can't kill a server", to));}
        irc::message * err_nooperhost(int to)														{ return (new irc::message(SERVER_NAME, SSTR(ERR_NOOPERHOST), 		"", 			"No O-lines for your host", to));}
        irc::message * err_umodeunknownflag(int to)							    					{ return (new irc::message(SERVER_NAME, SSTR(ERR_UMODEUNKNOWNFLAG),   "", 			"Unknown MODE flag", to));}
        irc::message * err_usersdontmatch(int to)													{ return (new irc::message(SERVER_NAME, SSTR(ERR_USERSDONTMATCH), 	"", 			"Can't change mode for other users", to));}
    }

    namespace rpl
    {
        irc::message * rpl_welcome(const str &nick, int to)									        { return (new irc::message(SERVER_NAME, "001", 			nick, 			"Welcome to the Internet Relay Network " + nick, to));}
        irc::message * rpl_yourhost(const str &nick, const str &srvname, const str &version, int to){ return (new irc::message(SERVER_NAME, "002", 		    nick, 			"Your host is " + srvname + ", running version " + version, to));}
        irc::message * rpl_created(const str &nick, const str &date, int to)						{ return (new irc::message(SERVER_NAME, "003", 			nick, 			"This server was created " + date, to));}
        irc::message * rpl_myinfo(const str &nick, const str &srvname, const str &version, const str &availableUserModes, const str &availableChannelModes, int to) 
                                                                                                    { return (new irc::message(SERVER_NAME, "004", 			nick, 			srvname + ' ' + version + ' ' + availableUserModes + ' ' + availableChannelModes, to));}
        irc::message * rpl_bounce(const str &nick, const str &srvname, const str &port, int to)	    { return (new irc::message(SERVER_NAME, "005", 			nick, 			"Try server " + srvname + ", port " + port, to));}
        // irc::message * rpl_statslinkinfo(const str &linkname, size_t sendq, size_t sentMessages, size_t sentKbytes, size_t recvMessages, size_t recvKbytes, time_t timeOpen, int to){}
        // irc::message * rpl_statscommands(const str &command, size_t count, size_t byteCount, size_t remoteCount, int to){}
        irc::message * rpl_endofstats(const str &letter, int to)									{ return (new irc::message(SERVER_NAME, SSTR(RPL_ENDOFSTATS), 		letter, 		"End of STATS report", to));}
        irc::message * rpl_umodeis(const str &nickname, const str &modes, int to)					{ return (new irc::message(SERVER_NAME, SSTR(RPL_UMODEIS), 		nickname,	'+' + modes, to));}
        // irc::message * rpl_statuptime(time_t uptime, int to)                                         { return (new irc::message(SERVER_NAME, "", "", to));}
        irc::message * rpl_statoline(const str &hostmask, const str &name, int to)					{ return (new irc::message(SERVER_NAME, SSTR(RPL_AWAY), 			    "O " + hostmask + " * " + name, "", to));}
        irc::message * rpl_away(const str &nickname, const str &reason, int to)						{ return (new irc::message(SERVER_NAME, SSTR(RPL_AWAY), 			    nickname, 		reason, to));}
        irc::message * rpl_unaway(int to)															{ return (new irc::message(SERVER_NAME, SSTR(RPL_UNAWAY), 			"", 			"You are no longer marked as being away", to));}
        irc::message * rpl_nowaway(int to)											    			{ return (new irc::message(SERVER_NAME, SSTR(RPL_AWAY), 			    "", 			"You have been marked as being away", to));}
        // irc::message * rpl_list(const str &channel, size_t visible, const str &topic, int to)		{ return (new irc::message(SERVER_NAME, SSTR(RPL_LIST), 			    channel + ' ' + SSTR(visible, topic), to));} // because SSTR not working here
        irc::message * rpl_listend(int to)											    			{ return (new irc::message(SERVER_NAME, SSTR(RPL_LISTEND), 			"", 			"End of LIST", to));}
        irc::message * rpl_channelmodeis(const str &channel, const str &modes, int to)				{ return (new irc::message(SERVER_NAME, SSTR(RPL_CHANNELMODEIS), 	    channel + " +" + modes, "", to));}
        irc::message * rpl_notopic(const str &channel, int to)										{ return (new irc::message(SERVER_NAME, SSTR(RPL_NOTOPIC), 			channel, 		"No topic is set", to));}
        irc::message * rpl_topic(const str &channel, const str &topic, int to)						{ return (new irc::message(SERVER_NAME, SSTR(RPL_TOPIC), 			    channel, 		topic, to));}
        irc::message * rpl_inviting(const str &channel, const str &nickname, int to)				{ return (new irc::message(SERVER_NAME, SSTR(RPL_INVITING), 		    channel + ' ' + nickname, "", to));}
        irc::message * rpl_version(const str &nickname, const str &version, const str debuglevel, const str &server, const str &comments, int to)	
                                                                                                    { return (new irc::message(SERVER_NAME, SSTR(RPL_VERSION), 			nickname,  version + '.' + debuglevel + ' ' + server + ' ' + comments, to));}
        irc::message * rpl_namreply(const str &names, int to)										{ return (new irc::message(SERVER_NAME, SSTR(RPL_NAMREPLY), 		    names, 			"", to));}
        irc::message * rpl_endofnames(const str &name, int to)										{ return (new irc::message(SERVER_NAME, SSTR(RPL_ENDOFNAMES), 		name, 			"End of NAMES list", to));}
        irc::message * rpl_banlist(const str &channel, const str &banmask, int to)					{ return (new irc::message(SERVER_NAME, SSTR(RPL_BANLIST), 			channel + ' ' + banmask, "", to));}
        irc::message * rpl_endofbanlist(const str &channel, int to)									{ return (new irc::message(SERVER_NAME, SSTR(RPL_ENDOFBANLIST), 	    channel, 		"End of channel ban list", to));}
        irc::message * rpl_motd(const str &text, int to)											{ return (new irc::message(SERVER_NAME, SSTR(RPL_MOTD), 			    "", 			"- " + text, to));}
        irc::message * rpl_motdstart(const str &srvname, int to)									{ return (new irc::message(SERVER_NAME, SSTR(RPL_MOTDSTART), 		    "", 			"- " + srvname + " Message of the day - ", to));}
        irc::message * rpl_endofmotd(int to)														{ return (new irc::message(SERVER_NAME, SSTR(RPL_ENDOFMOTD), 		    "", 			"End of MOTD command", to));}
        irc::message * rpl_info(const str &info, int to)											{ return (new irc::message(SERVER_NAME, SSTR(RPL_INFO), 			    "", 			info, to));}
        irc::message * rpl_endofinfo(int to)														{ return (new irc::message(SERVER_NAME, SSTR(RPL_ENDOFINFO), 		    "", 			"End of INFO list", to));}
        irc::message * rpl_youreoper(const str &nickname, int to)									{ return (new irc::message(SERVER_NAME, SSTR(RPL_YOUREOPER), 		    nickname, 		"You are now an IRC operator", to));}
        irc::message * rpl_time(const str &srvname, int to)										    { return (new irc::message(SERVER_NAME, SSTR(RPL_TIME), 			    srvname, 	ft_current_time(), to));} 
        irc::message * rpl_whoisuser(const str &nick, const str &user, const str &host, const str &realname, int to)	
                                                                                                    { return (new irc::message(SERVER_NAME, SSTR(RPL_WHOISUSER), 		    nick + " " + user + " " + host + " *", realname, to));}
        irc::message * rpl_whoisserver(const str &nick, const str &srvname, const str shortinfo, int to)
                                                                                                    { return (new irc::message(SERVER_NAME, SSTR(RPL_WHOISSERVER), 		nick + " " + srvname, shortinfo, to));}
        irc::message * rpl_whoisoperator(const str &nick, int to)									{ return (new irc::message(SERVER_NAME, SSTR(RPL_WHOISOPERATOR), 	    nick, 			"is an IRC operator", to));}
        irc::message * rpl_whoisidle(const str &nick, size_t idle, int to)							{ return (new irc::message(SERVER_NAME, SSTR(RPL_WHOISIDLE), 		    nick + " " + SSTR(idle), "seconds idle", to));}
        irc::message * rpl_whoischannels(const str &nick, const str &status, const str &channel, int to)	
                                                                                                    { return (new irc::message(SERVER_NAME, SSTR(RPL_WHOISCHANNELS), 	    nick, 			status + channel, to));}
        irc::message * rpl_endofwhois(const str &nick, int to)										{ return (new irc::message(SERVER_NAME, SSTR(RPL_ENDOFWHOIS), 		nick, 			"End of WHOIS list", to));}
        irc::message * rpl_whoreply(const str &msg, int to)											{ return (new irc::message(SERVER_NAME, SSTR(RPL_WHOREPLY), 		    msg, 			"", to));}
        irc::message * rpl_endofwho(const str &nickname, const str &target, int to)					{ return (new irc::message(SERVER_NAME, SSTR(RPL_ENDOFWHO), 		nickname + " "  +  target, 		"End of /WHO list", to));}
        irc::message * rpl_whowasuser(const str &nick, const str &user, const str &host, const str &realname, int to)	
                                                                                                    { return (new irc::message(SERVER_NAME, SSTR(RPL_WHOWASUSER), 		nick + " " + user + " " + host + " *", realname, to));}
        irc::message * rpl_endofwhowas(const str &nick, int to)										{ return (new irc::message(SERVER_NAME, SSTR(RPL_ENDOFWHOWAS), 		nick, 			"End of WHOWAS", to));}
        irc::message * rpl_userhostreply(const str &msg, int to)									{ return (new irc::message(SERVER_NAME, SSTR(RPL_USERHOST), 		    "", 			msg, to));}
    }

    namespace cmd
    {
        irc::message * cmd_ping(const str &nick, int to)                                            { return (new irc::message(SERVER_NAME, "PING", nick, SERVER_NAME, to));}
        irc::message * cmd_mode(const str &target, const str &param, int to)                        { return (new irc::message(SERVER_NAME, "MODE", target, param, to));}
        irc::message * cmd_nick(const str &user_prefix, const str &nick, int to)                    { return (new irc::message(user_prefix, "NICK", "", nick, to));}   
    } 

}