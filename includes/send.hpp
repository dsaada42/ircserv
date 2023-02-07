/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 17:06:15 by dsaada            #+#    #+#             */
/*   Updated: 2023/02/07 17:31:40 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEND_HPP
# define SEND_HPP
# include "ircserv.hpp"
# include "message.hpp"
# include "utils.hpp"

namespace irc{

    enum num_code
    {
        ERR_NOSUCHNICK = 401,
        ERR_NOSUCHSERVER = 402,
        ERR_NOSUCHCHANNEL = 403,
        ERR_CANNOTSENDTOCHAN = 404,
        ERR_TOOMANYCHANNELS = 405,
        ERR_WASNOSUCHNICK = 406,
        ERR_TOOMANYTARGETS = 407,
        ERR_NOORIGIN = 409,
        ERR_NORECIPIENT = 411,
        ERR_NOTEXTTOSEND = 412,
        ERR_NOTOPLEVEL = 413,
        ERR_WILDTOPLEVEL = 414,
        ERR_UNKNOWNCOMMAND = 421,
        ERR_NOMOTD = 422,
        ERR_NOADMINFO = 423,
        ERR_NONICKNAMEGIVEN = 431,
        ERR_ERRONEUSNICKNAME = 432,
        ERR_NICKNAMEINUSE = 433,
        ERR_USERNOTINCHANNEL = 441,
        ERR_NOTONCHANNEL = 442,
        ERR_USERONCHANNEL = 443,
        ERR_SUMMONDISABLED = 445,
        ERR_USERSDISABLED = 446,
        ERR_NOTREGISTERED = 451,
        ERR_NEEDMOREPARAMS = 461,
        ERR_ALREADYREGISTRED = 462,
        ERR_NOPERMFORHOST = 463,
        ERR_PASSWDMISSMATCH = 464,
        ERR_YOUREBANNEDCREEP = 465,
        ERR_KEYSET = 467,
        ERR_CHANNELISFULL = 471,
        ERR_UNKNOWNMODE = 472,
        ERR_INVITEONLYCHAN = 473,
        ERR_BANNEDFROMCHAN = 474,
        ERR_BADCHANNELKEY = 475,
        ERR_NOPRIVILEGES = 481,
        ERR_CHANOPRIVSNEEDED = 482,
        ERR_CANTKILLSERVER = 483,
        ERR_NOOPERHOST = 491,
        ERR_UMODEUNKNOWNFLAG = 501,
        ERR_USERSDONTMATCH = 502,

        RPL_WELCOME = 001,
        RPL_YOURHOST = 002,
        RPL_CREATED = 003,
        RPL_MYINFO = 004,
        RPL_BOUNCE = 005,
        RPL_STATSLINKINFO = 211,
        RPL_STATSCOMMANDS = 212,
        RPL_ENDOFSTATS = 219,
        RPL_UMODEIS = 221,
        RPL_STATSLLINE = 241,
        RPL_STATSUPTIME = 242,
        RPL_USERHOST = 302,
        RPL_WHOISUSER = 311,
        RPL_WHOISSERVER = 312,
        RPL_WHOISOPERATOR = 313,
        RPL_WHOWASUSER = 314,
        RPL_ENDOFWHO = 315,
        RPL_WHOISIDLE = 317,
        RPL_ENDOFWHOIS = 318,
        RPL_WHOISCHANNELS = 319,
        RPL_LISTSTART = 321, 
        RPL_LIST = 322,
        RPL_LISTEND = 323,
        RPL_CHANNELMODEIS = 324,
        RPL_NOTOPIC = 331,
        RPL_TOPIC = 332,
        RPL_INVITING = 341,
        RPL_VERSION = 351,
        RPL_WHOREPLY = 352,
        RPL_NAMREPLY = 353,
        RPL_ENDOFNAMES = 366,
        RPL_BANLIST = 367,
        RPL_ENDOFBANLIST = 368,
        RPL_ENDOFWHOWAS = 369,
        RPL_INFO = 371,
        RPL_MOTD = 372,
        RPL_ENDOFINFO = 374,
        RPL_MOTDSTART = 375,
        RPL_ENDOFMOTD = 376,
        RPL_YOUREOPER = 381,
        RPL_TIME = 391,
    };

    namespace err
    {
        irc::message * err_nosuchnick(const str &nickname, int to);
        irc::message * err_nosuchserver(const str &srvname, int to);
        irc::message * err_nosuchchannel(const str &channelname, int to);
        irc::message * err_cannotsendtochan(const str &channelname, int to);
        irc::message * err_toomanychannels(const str &channelname, int to);
        irc::message * err_wasnosuchnick(const str &sender, const str &nickname, int to);
        irc::message * err_toomanytargets(const str &target, int to);
        irc::message * err_noorigin(int to);
        irc::message * err_norecipient(const str &command, int to);
        irc::message * err_notexttosend(int to);
        irc::message * err_notoplevel(const str &mask, int to);
        irc::message * err_wildtoplevel(const str &mask, int to);
        irc::message * err_unknowncommand(const str &command, int to);
        irc::message * err_nomotd(int to);
        irc::message * err_noadmininfo(int to);
        irc::message * err_nonicknamegiven(int to);
        irc::message * err_erroneusnickname(const str &nickname, int to);
        irc::message * err_nicknameinuse(const str &nickname, int to);
        irc::message * err_usernotinchannel(const str &nickname, const str &target, const str &channel, int to);
        irc::message * err_notonchannel(const str &channel, int to);
        irc::message * err_useronchannel(const str &nickname, const str &channel, int to);
        irc::message * err_summondisabled(int to);
        irc::message * err_usersdisabled(int to);
        irc::message * err_notregistered(int to);
        irc::message * err_needmoreparams(const str &command, int to);
        irc::message * err_alreadyregistred(int to);
        irc::message * err_nopermforhost(int to);
        irc::message * err_passwdmissmatch(int to);
        irc::message * err_yourebannedcreep(int to);
        irc::message * err_keyset(const str &channel, int to);
        irc::message * err_channelisfull(const str &channel, int to);
        irc::message * err_unknownmode(const str &nickname, const str &c, int to);
        irc::message * err_inviteonlychan(const str &nickname, const str &channel, int to);
        irc::message * err_bannedfromchan(const str &channel, int to);
        irc::message * err_badchannelkey(const str &nickname, const str &channel, int to);
        irc::message * err_noprivileges(int to);
        irc::message * err_chanoprivsneeded(const str &channel, int to);
        irc::message * err_cantkillserver(int to);
        irc::message * err_nooperhost(int to);
        irc::message * err_umodeunknownflag(int to);
        irc::message * err_usersdontmatch(int to);
    }

    namespace rpl
    {
        irc::message * rpl_welcome(const str &nick, int to);
        irc::message * rpl_yourhost(const str &nick, const str &srvname, const str &version, int to);
        irc::message * rpl_created(const str &nick, const str &date, int to);
        irc::message * rpl_myinfo(const str &nick, const str &srvname, const str &version, const str &umodes, const str &cmodes, int to);
        irc::message * rpl_bounce(const str &nick, const str &srvname, const str &port, int to);
        irc::message * rpl_statslinkinfo(const str &nickname, const str &stats, int to);
        irc::message * rpl_statscommands(const str &nickname, const str &command, int to);        
        irc::message * rpl_endofstats(const str &nickname, const str &letter, int to);
        irc::message * rpl_umodeis(const str &nickname, const str &modes, int to);
        irc::message * rpl_statsuptime(const str &nickname, const str &stats, int to);
        irc::message * rpl_liststart(int to);
        irc::message * rpl_list(const str &nick, const str &channel, const str &topic, int to);
        irc::message * rpl_listend(int to);
        irc::message * rpl_channelmodeis(const str &nickname, const str &channel, const str &modes, int to);
        irc::message * rpl_notopic(const str &nickname, const str &channel, int to);
        irc::message * rpl_topic(const str &nickname, const str &channel, const str &topic, int to);
        irc::message * rpl_inviting(const str &channel, const str &nickname, const str &target, int to);
        irc::message * rpl_version(const str &nickname, const str &version, const str debuglevel, const str &server, const str &comments, int to);
        irc::message * rpl_namreply(const str &nickname, const str &channel_name, const str &names, int to);
        irc::message * rpl_endofnames(const str &name, const str &channel_name, int to);
        irc::message * rpl_banlist(const str &channel, const str &banmask, int to);
        irc::message * rpl_endofbanlist(const str &channel, int to);
        irc::message * rpl_motd(const str &text, int to);
        irc::message * rpl_motdstart(const str &srvname, int to);
        irc::message * rpl_endofmotd(int to);
        irc::message * rpl_info(const str &info, int to);
        irc::message * rpl_endofinfo(int to);
        irc::message * rpl_youreoper(const str &nickname, int to);
        irc::message * rpl_time(const str &srvname, int to);
        irc::message * rpl_whoisuser(const str &sender, const str &nickname, const str &username, const str &host, const str &fullname, int to);
        irc::message * rpl_whoisserver(const str &sender, const str &nick, const str &srvname, const str shortinfo, int to);
        irc::message * rpl_whoisoperator(const str &nick, int to);
        irc::message * rpl_whoisidle(const str &sender, const str &nickname, const str &idle, const str &timestamp, int to);
        irc::message * rpl_whoischannels(const str &sender, const str &nickname, const str &chans, int to);
        irc::message * rpl_endofwhois(const str &sender, const str &nicks, int to);
        irc::message * rpl_whoreply(const str &sender, const str &msg, const str &trailing, int to);
        irc::message * rpl_endofwho(const str &nickname, const str &target, int to);
        irc::message * rpl_whowasuser(const str & sender, const str &nick, const str &user, const str &host, const str &realname, int to);
        irc::message * rpl_endofwhowas(const str &sender, const str &nick, int to);
        irc::message * rpl_userhostreply(const str &msg, int to);
    }

    namespace cmd
    {
        irc::message * cmd_ping(const str &nick, int to);
        irc::message * cmd_mode(const str &target, const str &param, int to);
        irc::message * cmd_nick(const str &user_prefix, const str &nick, int to);                  
        irc::message * cmd_quit(const str &user_prefix, const str &msg, int to);
        irc::message * cmd_privmsg(const str &user_prefix, const str &target, const str &msg, int to);
        irc::message * cmd_notice(const str &user_prefix, const str &target, const str &msg, int to);
	    irc::message * cmd_join(const str &user_prefix, const str &channel_name, int to);
        irc::message * cmd_invite(const str &user_prefix, const str &target, const str &channel_name, int to);
        irc::message * cmd_part(const str &user_prefix, const str &channel_name, const str &msg, int to);
        irc::message * cmd_mode_channel(const str &user_prefix, const str &channel_name, const str &mode, int to);
        irc::message * cmd_pong(const str &param, int to);
    }  
    
}

#endif
