/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 17:07:50 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/25 18:16:06 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "send.hpp"

namespace irc{
    
    namespace error
    {
        irc::message * err_nosuchnick(const str &nickname, int to)									{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_NOSUCHNICK), 		nickname, 		"No such nick/channel", to)); } 
        irc::message * err_nosuchserver(const str &servername, int to)								{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_NOSUCHSERVER), 	    servername, 	"No such server", to));} 
        irc::message * err_nosuchchannel(const str &channelname, int to)							{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_NOSUCHCHANNEL), 	    channelname, 	"No such channel", to));} 
        irc::message * err_cannotsendtochan(const str &channelname, int to)							{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_CANNOTSENDTOCHAN),   channelname, 	"Cannot send to channel", to));} 
        irc::message * err_toomanychannels(const str &channelname, int to)							{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_TOOMANYCHANNELS), 	channelname, 	"You have joined too many channels", to));}
        irc::message * err_wasnosuchnick(const str &nickname, int to)								{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_WASNOSUCHNICK), 	    nickname, 		"There was no such nickname", to));}
        irc::message * err_toomanytargets(const str &target, int to)								{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_TOOMANYTARGETS), 	target, 		"Duplicate recipients. No message delivered", to));}
        // irc::message * err_nosuchservice(const str &servicename, int to)							{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_NOSUCHSERVICE), 	servicename, 	"No such service", to));}
        irc::message * err_noorigin(int to)														    { return (new irc::message(SERVER_PREFIX, SSTR(ERR_NOORIGIN), 		    "", 			"No origin specified", to));}
        irc::message * err_norecipient(const str &command, int to)									{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_NORECIPIENT), 		"", 			"No recipient given " + command, to));}
        irc::message * err_notexttosend(int to) 													{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_NOTEXTTOSEND), 	    "", 			"No text to send", to));}
        irc::message * err_notoplevel(const str &mask, int to)										{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_NOTOPLEVEL), 		mask, 			"No top level domain specified", to));}
        irc::message * err_wildtoplevel(const str &mask, int to)									{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_WILDTOPLEVEL), 	    mask, 			"Wildcard in top level domain", to));}
        // irc::message * err_badmask(const str &mask, int to)											{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_BADMASK), 			mask, 			"Bad Server/host mask", to));}
        irc::message * err_unknowncommand(const str &command, int to)								{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_UNKNOWNCOMMAND), 	command, 		"Unknown command", to));}
        irc::message * err_nomotd(int to)   														{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_NOMOTD), 			"", 			"MOTD file is missing", to));}
        irc::message * err_noadmininfo(int to)  													{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_NOADMINFO), 		    "", 			"No administrative info available", to));}
        irc::message * err_fillerror(const str &file_op, const str &file, int to)					{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_FILERROR), 		    "", 			"File error doing " + file_op + " on " + file, to));}
        irc::message * err_nonicknamegiven(int to)	    											{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_NONICKNAMEGIVEN), 	"", 			"No nickame given", to));}
        irc::message * err_erroneusnickname(const str &nickname, int to)							{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_ERRONEUSNICKNAME),   nickname, 		"Erroneus nickname", to));}
        irc::message * err_nicknameinuse(const str &nickname, int to)								{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_NICKNAMEINUSE), 	    nickname,	 	"Nickname is already in use", to));}
        irc::message * err_nickcollision(const str &nickname, int to)								{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_NICKCOLLISION), 	    nickname,		"Nickname collision KILL", to));}
        irc::message * err_usernotinchannel(const str &nickname, const str &channel, int to)		{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_USERNOTINCHANNEL),   nickname + ' ' + channel, "They aren't on that channel", to));}
        irc::message * err_notonchannel(const str &channel, int to)									{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_NOTONCHANNEL), 	    channel, 		"You're not on that channel", to));}
        irc::message * err_useronchannel(const str &nickname, const str &channel, int to)			{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_USERONCHANNEL), 	    nickname + ' ' + channel, "is already on channel", to));}
        irc::message * err_nologin(const str &user, int to)											{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_NOLOGIN), 			user,	 		"User not logged in", to));}
        irc::message * err_summondisabled(int to)													{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_SUMMONDISABLED), 	"", 			"Summon has been disabled", to));}
        irc::message * err_usersdisabled(int to) 													{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_USERDISABLED), 	    "", 			"USERS has been disabled", to));}
        irc::message * err_notregistered(int to) 													{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_NOTREGISTERED), 	    "", 			"You have not registered", to));}
        irc::message * err_needmoreparams(const str &command, int to)								{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_NEEDMOREPARAMS), 	command, 		"Not enough parameters", to));}
        irc::message * err_alreadyregistred(int to)					    							{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_ALREADYREGISTRED),	"", 			"Unauthorized command (already registered)", to));}
        irc::message * err_nopermforhost(int to)													{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_NOPERMFORHOST), 	    "", 			"Your host is not among the privileged", to));}
        irc::message * err_passwdmissmatch(int to) 						    						{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_PASSWDMISSMATCH), 	"", 			"Password incorrect", to));}
        irc::message * err_yourebannedcreep(int to)							    					{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_YOUREBANNEDCREEP),   "", 			"You are banned from this server", to));}
        irc::message * err_keyset(const str &channel, int to)										{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_KEYSET), 			channel, 		"Channel key already set", to));}
        irc::message * err_channelisfull(const str &channel, int to)								{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_CHANNELISFULL), 	    channel, 		"Channel is full", to));}
        irc::message * err_unknownmode(const str &c, int to)										{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_UNKNOWNMODE), 		c, 				"is unknown mode char to me", to));}
        irc::message * err_inviteonlychan(const str &channel, int to)								{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_INVITEONLYCHAN), 	channel, 		"Cannot join channel (+i)", to));}
        irc::message * err_bannedfromchan(const str &channel, int to)								{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_BANNEDFROMCHAN), 	channel, 		"Cannot join channel (+b)", to));}
        irc::message * err_badchannelkey(const str &channel, int to)								{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_BADCHANNELKEY), 	    channel, 		"Cannot join channel (+k)", to));}
        // irc::message * err_nochanmodes(const str &channel, int to)									{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_NOCHANMODES), 		channel, 		"Channel doesn't support modes", to));}
        irc::message * err_noprivileges(int to)						    							{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_NOPRIVILEGES), 	    "", 			"Permission Denied- You're not an IRC operator", to));}
        irc::message * err_chanoprisneeded(const str &channel, int to)								{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_CHANOPRIVISNEED), 	channel, 		"You're not channel operator", to));}
        irc::message * err_cantkillserver(int to) 						    						{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_CANTKILLSERVER), 	"", 			"You can't kill a server", to));}
        // irc::message * err_restricted(int to)														{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_RESTRICTED), 		"", 			"Your connection is restricted!", to));}
        irc::message * err_nooperhost(int to)														{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_NOOPERHOST), 		"", 			"No O-lines for your host", to));}
        irc::message * err_umodeunknownflag(int to)							    					{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_UMODEUNKNOWNFLAG),   "", 			"Unknown MODE flag", to));}
        irc::message * err_usersdontmatch(int to)													{ return (new irc::message(SERVER_PREFIX, SSTR(ERR_USERSDONTMATCH), 	"", 			"Can't change mode for other users", to));}
    } /* end of namespace Error */

    namespace num
    {
        irc::message * rpl_welcome(const str &userPrefix, int to)									{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_WELCOME), 			"", 			"Welcome to the Internet Relay Network " + userPrefix, to));}
        irc::message * rpl_yourhost(const str &servername, const str &version, int to)				{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_YOURHOST), 		    "", 			"Your host is " + servername + ", running version " + version, to));}
        irc::message * rpl_created(const str &date, int to)											{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_CREATED), 			"", 			"This server was created " + date, to));}
        irc::message * rpl_myinfo(const str &servername, const str &version, const str &availableUserModes, const str &availableChannelModes, int to) 
                                                                                                    { return (new irc::message(SERVER_PREFIX, SSTR(RPL_MYINFO), 			"", 			servername + ' ' + version + ' ' + availableUserModes + ' ' + availableChannelModes, to));}
        irc::message * rpl_bounce(const str &servername, const str &port, int to)					{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_BOUNCE), 			"", 			"Try server " + servername + ", port " + port, to));}
    //     irc::message * rpl_statslinkinfo(const str &linkname, size_t sendq, size_t sentMessages, size_t sentKbytes, size_t recvMessages, size_t recvKbytes, time_t timeOpen, int to){
    //         strstream ss;
    //         ss << linkname << ' ' << sendq << ' ' << sentMessages << ' ' << sentKbytes
    //         << ' ' << recvMessages << ' ' << recvKbytes << ' ' << timeOpen;
    //         return (new irc::message(SERVER_PREFIX, SSTR(RPL_STATSLINKINFO), ss.str(), to));
    // }
    //     irc::message * rpl_statscommands(const str &command, size_t count, size_t byteCount, size_t remoteCount, int to){
    //         strstream ss;
    //         ss << command << ' ' << count << ' ' << byteCount << ' ' << remoteCount;
    //         return (new irc::message(SERVER_PREFIX, SSTR(RPL_STATSCOMMANDS), ss.str(), to));
    // }
        irc::message * rpl_endofstats(const str &letter, int to)									{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_ENDOFSTATS), 		letter, 		"End of STATS report", to));}
        irc::message * rpl_umodeis(const str &modes, int to)										{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_UMODEIS), 			'+' + modes, 	"", to));}
        // irc::message * rpl_servlist(const str &name, const str &server, const str &mask, const str &type, size_t hopcount, const str &info, int to)
                                                                                                    // { return (new irc::message(SERVER_PREFIX, SSTR(RPL_SERVLIST), 		    name + ' ' + server + ' ' + mask + ' ' + type + ' ' + SSTR(hopcount), info, to));}
        // irc::message * rpl_servlistend(const str &mask, const str &type, int to)					{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_SERVLISTEND), 		mask + ' ' + type, "End of service listing", to));}
        // irc::message * rpl_statuptime(time_t uptime, int to){
        //     strstream ss;
        //     time_t m = 60, h = 60 * m, d = 24 * h;
        //     ss << ":Server Up " << (uptime / d) << " days ";
        //     uptime %= d;
        //     ss << (uptime / h, to);
        //     uptime %= h;
        //     ss << ':' << std::setfill('0') << std::setw(2) << (uptime / m)
        //     << ':' << std::setfill('0') << std::setw(2) << (uptime % m, to);
        //     return (new irc::message(SERVER_PREFIX, SSTR(RPL_STATSUPTIME), ss.str(), to));
        // }
        irc::message * rpl_statoline(const str &hostmask, const str &name, int to)					{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_AWAY), 			    "O " + hostmask + " * " + name, "", to));}
        irc::message * rpl_away(const str &nickname, const str &reason, int to)						{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_AWAY), 			    nickname, 		reason, to));}
        irc::message * rpl_unaway(int to)															{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_UNAWAY), 			"", 			"You are no longer marked as being away", to));}
        irc::message * rpl_nowaway(int to)											    			{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_AWAY), 			    "", 			"You have been marked as being away", to));}
        // irc::message * rpl_list(const str &channel, size_t visible, const str &topic, int to)		{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_LIST), 			    channel + ' ' + SSTR(visible, topic), to));} // because SSTR not working here
        irc::message * rpl_listend(int to)											    			{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_LISTEND), 			"", 			"End of LIST", to));}
        irc::message * rpl_channelmodeis(const str &channel, const str &modes, int to)				{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_CHANNELMODEIS), 	    channel + " +" + modes, "", to));}
        irc::message * rpl_notopic(const str &channel, int to)										{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_NOTOPIC), 			channel, 		"No topic is set", to));}
        irc::message * rpl_topic(const str &channel, const str &topic, int to)						{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_TOPIC), 			    channel, 		topic, to));}
        irc::message * rpl_inviting(const str &channel, const str &nickname, int to)				{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_INVITING), 		    channel + ' ' + nickname, "", to));}
        // irc::message * rpl_invitelist(const str &channel, const str &invitemask, int to)			{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_INVITELIST), 		channel + ' ' + invitemask, "", to));}
        // irc::message * rpl_endofinvitelist(const str &channel, int to)								{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_ENDOFINVITELIST), 	channel, 		"End of channel invite list", to));}
        // irc::message * rpl_exceptlist(const str &channel, const str &exceptionmask, int to)			{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_EXCEPTLIST), 		channel + ' ' + exceptionmask, "", to));}
        // irc::message * rpl_endofexceptlist(const str &channel, int to)								{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_ENDOFEXCEPTLIST), 	channel, 		"End of channel exception list", to));}
        irc::message * rpl_version(const str &version, const str debuglevel, const str &server, const str &comments, int to)	
                                                                                                    { return (new irc::message(SERVER_PREFIX, SSTR(RPL_VERSION), 			version + '.' + debuglevel + ' ' + server, comments, to));}
        irc::message * rpl_namreply(const str &names, int to)										{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_NAMREPLY), 		    names, 			"", to));}
        irc::message * rpl_endofnames(const str &name, int to)										{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_ENDOFNAMES), 		name, 			"End of NAMES list", to));}
        irc::message * rpl_banlist(const str &channel, const str &banmask, int to)					{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_BANLIST), 			channel + ' ' + banmask, "", to));}
        irc::message * rpl_endofbanlist(const str &channel, int to)									{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_ENDOFBANLIST), 	    channel, 		"End of channel ban list", to));}
        irc::message * rpl_motd(const str &text, int to)											{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_MOTD), 			    "", 			"- " + text, to));}
        irc::message * rpl_motdstart(const str &servername, int to)									{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_MOTDSTART), 		    "", 			"- " + servername + " Message of the day - ", to));}
        irc::message * rpl_endofmotd(int to)														{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_ENDOFMOTD), 		    "", 			"End of MOTD command", to));}
        irc::message * rpl_info(const str &info, int to)											{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_INFO), 			    "", 			info, to));}
        irc::message * rpl_endofinfo(int to)														{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_ENDOFINFO), 		    "", 			"End of INFO list", to));}
        irc::message * rpl_youreoper(int to)														{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_YOUREOPER), 		    "", 			"You are now an IRC operator", to));}
        irc::message * rpl_youreservice(const str &servicename, int to)								{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_YOURESERVICE), 	    "", 			"You are service " + servicename, to));}
        // irc::message * rpl_time(const str &servername, int to)										{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_TIME), 			    servername, 	ft::toDate(::time(""), " :%x - %I:%M:%S %z"), to));} // formatter le time autrement
        irc::message * rpl_lusersclient(size_t users, size_t services, size_t servers, int to)		{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_LUSERCLIENT), 		"", 			"There are " + SSTR(users) + " users and " + SSTR(services) + " services on " + SSTR(servers) + " server", to));	}
        irc::message * rpl_lusersop(size_t nb, int to)												{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_LUSEROP), 			SSTR(nb), "operator(s) online", to));}
        irc::message * rpl_lusersunknown(size_t nb, int to)											{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_LUSERUNKNOWN), 	    SSTR(nb), "unknown connection(s)", to));}
        irc::message * rpl_luserschannels(size_t nb, int to)										{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_LUSERCHANNELS), 	    SSTR(nb), "channels formed", to));}
        irc::message * rpl_lusersme(size_t nb, int to)												{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_LUSERME), 			"", 			"I have " + SSTR(nb) + " clients and 1 server", to));}
        irc::message * rpl_whoisuser(const str &nick, const str &user, const str &host, const str &realname, int to)	
                                                                                                    { return (new irc::message(SERVER_PREFIX, SSTR(RPL_WHOISUSER), 		    nick + " " + user + " " + host + " *", realname, to));}
        irc::message * rpl_whoisserver(const str &nick, const str &servername, const str shortinfo, int to)
                                                                                                    { return (new irc::message(SERVER_PREFIX, SSTR(RPL_WHOISSERVER), 		nick + " " + servername, shortinfo, to));}
        irc::message * rpl_whoisoperator(const str &nick, int to)									{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_WHOISOPERATOR), 	    nick, 			"is an IRC operator", to));}
        irc::message * rpl_whoisidle(const str &nick, size_t idle, int to)							{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_WHOISIDLE), 		    nick + " " + SSTR(idle), "seconds idle", to));}
        irc::message * rpl_whoischannels(const str &nick, const str &status, const str &channel, int to)	
                                                                                                    { return (new irc::message(SERVER_PREFIX, SSTR(RPL_WHOISCHANNELS), 	    nick, 			status + channel, to));}
        irc::message * rpl_endofwhois(const str &nick, int to)										{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_ENDOFWHOIS), 		nick, 			"End of WHOIS list", to));}
        irc::message * rpl_whoreply(const str &msg, int to)											{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_WHOREPLY), 		    msg, 			"", to));}
        irc::message * rpl_endofwho(const str &target, int to)										{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_ENDOFWHO), 		    target, 		"End of WHO list", to));}
        irc::message * rpl_whowasuser(const str &nick, const str &user, const str &host, const str &realname, int to)	
                                                                                                    { return (new irc::message(SERVER_PREFIX, SSTR(RPL_WHOWASUSER), 		nick + " " + user + " " + host + " *", realname, to));}
        irc::message * rpl_endofwhowas(const str &nick, int to)										{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_ENDOFWHOWAS), 		nick, 			"End of WHOWAS", to));}
        irc::message * rpl_rehashing(const str &filename, int to)									{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_REHASHING), 		    filename, 		"Rehashing", to));}
        irc::message * rpl_userhostreply(const str &msg, int to)									{ return (new irc::message(SERVER_PREFIX, SSTR(RPL_USERHOST), 		    "", 			msg, to));}
    }

    namespace cmd
    {
        irc::message * cmd_ping(int to)                                                             { return (new irc::message(SERVER_PREFIX, "PING", "", "", to));}
    } 

}