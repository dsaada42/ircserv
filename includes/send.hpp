/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 17:06:15 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/26 08:09:43 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEND_HPP
# define SEND_HPP
# include "ircserv.hpp"
# include "message.hpp"

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
        // ERR_NOSUCHSERVICE = 408,
        ERR_NOORIGIN = 409,
        ERR_NORECIPIENT = 411,
        ERR_NOTEXTTOSEND = 412,
        ERR_NOTOPLEVEL = 413,
        ERR_WILDTOPLEVEL = 414,
        // ERR_BADMASK = 415,
        ERR_UNKNOWNCOMMAND = 421,
        ERR_NOMOTD = 422,
        ERR_NOADMINFO = 423,
        ERR_FILERROR = 424,
        ERR_NONICKNAMEGIVEN = 431,
        ERR_ERRONEUSNICKNAME = 432,
        ERR_NICKNAMEINUSE = 433,
        ERR_NICKCOLLISION = 436,
        // ERR_UNAVAILABLE_RESOURCE = 437,
        ERR_USERNOTINCHANNEL = 441,
        ERR_NOTONCHANNEL = 442,
        ERR_USERONCHANNEL = 443,
        ERR_NOLOGIN = 444,
        ERR_SUMMONDISABLED = 445,
        ERR_USERDISABLED = 446,
        ERR_NOTREGISTERED = 451,
        ERR_NEEDMOREPARAMS = 461,
        ERR_ALREADYREGISTRED = 462,
        ERR_NOPERMFORHOST = 463,
        ERR_PASSWDMISSMATCH = 464,
        ERR_YOUREBANNEDCREEP = 465,
        // ERR_YOUWILLBEBANNED = 466,
        ERR_KEYSET = 467,
        ERR_CHANNELISFULL = 471,
        ERR_UNKNOWNMODE = 472,
        ERR_INVITEONLYCHAN = 473,
        ERR_BANNEDFROMCHAN = 474,
        ERR_BADCHANNELKEY = 475,
        // ERR_BADCHANMASK = 476,
        // ERR_NOCHANMODES = 477,
        // ERR_BANNLISTFULL = 478,
        ERR_NOPRIVILEGES = 481,
        ERR_CHANOPRIVISNEED = 482,
        ERR_CANTKILLSERVER = 483,
        // ERR_RESTRICTED = 484,
        // ERR_UNIQOPPRIVSNEEDED = 485,
        ERR_NOOPERHOST = 491,
        ERR_UMODEUNKNOWNFLAG = 501,
        ERR_USERSDONTMATCH = 502,

        RPL_WELCOME = 1,
        RPL_YOURHOST = 2,
        RPL_CREATED = 3,
        RPL_MYINFO = 4,
        RPL_BOUNCE = 5,
        RPL_STATSLINKINFO = 211,
        RPL_STATSCOMMANDS = 212,
        RPL_STATSCLINE = 213,//==========?
        RPL_STATSNLINE = 214,//==========?
        RPL_STATSILINE = 215,//==========?
        RPL_STATSKLINE = 216,//==========?
        RPL_STATSYLINE = 218,//==========?
        RPL_ENDOFSTATS = 219,
        RPL_UMODEIS = 221,
        // RPL_SERVLIST = 234,
        // RPL_SERVLISTEND = 235,
        RPL_STATSLLINE = 241,//==========?
        RPL_STATSUPTIME = 242,
        RPL_STATSOLINE = 243,
        RPL_STATSHLINE = 244, //==========?
        RPL_LUSERCLIENT = 251,
        RPL_LUSEROP = 252,
        RPL_LUSERUNKNOWN = 253,
        RPL_LUSERCHANNELS = 254,
        RPL_LUSERME = 255,
        RPL_ADMINME = 256, //==========?
        RPL_ADMINLOC1 = 257, //==========?
        RPL_ADMINLOC2 = 258, //==========?
        RPL_ADMINEMAIL = 259, //==========?
        // RPL_NONE = 300, 
        RPL_AWAY = 301,
        RPL_USERHOST = 302,
        RPL_ISON = 303,
        RPL_UNAWAY = 305,
        RPL_NOWAWAY = 306,
        RPL_WHOISUSER = 311,
        RPL_WHOISSERVER = 312,
        RPL_WHOISOPERATOR = 313,
        RPL_WHOWASUSER = 314,
        RPL_ENDOFWHO = 315,
        RPL_WHOISIDLE = 317,
        RPL_ENDOFWHOIS = 318,
        RPL_WHOISCHANNELS = 319,
        RPL_LISTSTART = 321, /* Not used / (╯°□°）╯︵ ┻━┻  */
        RPL_LIST = 322,
        RPL_LISTEND = 323,
        RPL_CHANNELMODEIS = 324,
        RPL_NOTOPIC = 331,
        RPL_TOPIC = 332,
        RPL_INVITING = 341,
        RPL_SUMMONING = 342,
        // RPL_INVITELIST = 346,
        // RPL_ENDOFINVITELIST =  347,
        // RPL_EXCEPTLIST = 348,
        // RPL_ENDOFEXCEPTLIST = 349,
        RPL_VERSION = 351,
        RPL_WHOREPLY = 352,
        RPL_NAMREPLY = 353,
        RPL_LINKS = 364, //========?
        RPL_ENDOFLINKS = 365, //========?
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
        RPL_REHASHING = 382,
        RPL_YOURESERVICE = 383,
        RPL_TIME = 391,
        RPL_USERSSTART = 392, //========?
        RPL_USERS = 393, //========?
        RPL_ENDOFUSERS = 394, //========?
        RPL_NOUSERS = 395, //========?
    };

    namespace error
    {
        irc::message * err_nosuchnick(const str &nickname, int to);
        irc::message * err_nosuchserver(const str &servername, int to);
        irc::message * err_nosuchchannel(const str &channelname, int to);
        irc::message * err_cannotsendtochan(const str &channelname, int to);
        irc::message * err_toomanychannels(const str &channelname, int to);
        irc::message * err_wasnosuchnick(const str &nickname, int to);
        irc::message * err_toomanytargets(const str &target, int to);
        irc::message * err_nosuchservice(const str &servicename, int to);
        irc::message * err_noorigin(int to);
        irc::message * err_norecipient(const str &command, int to);
        irc::message * err_notexttosend(int to);
        irc::message * err_notoplevel(const str &mask, int to);
        irc::message * err_badmask(const str &mask, int to);
        irc::message * err_wildtoplevel(const str &mask, int to);
        irc::message * err_unknowncommand(const str &command, int to);
        irc::message * err_nomotd(int to);
        irc::message * err_noadmininfo(int to);
        irc::message * err_fillerror(const str &file_op, const str &file, int to);
        irc::message * err_nonicknamegiven(int to);
        irc::message * err_erroneusnickname(const str &nickname, int to);
        irc::message * err_nicknameinuse(const str &nickname, int to);
        irc::message * err_nickcollision(const str &nickname, int to);
        irc::message * err_usernotinchannel(const str &nickname, const str &channel, int to);
        irc::message * err_notonchannel(const str &channel, int to);
        irc::message * err_useronchannel(const str &nickname, const str &channel, int to);
        irc::message * err_nologin(const str &user, int to);
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
        irc::message * err_unknownmode(const str &c, int to);
        irc::message * err_inviteonlychan(const str &channel, int to);
        irc::message * err_bannedfromchan(const str &channel, int to);
        irc::message * err_badchannelkey(const str &channel, int to);
        irc::message * err_nochanmodes(const str &channel, int to);
        irc::message * err_noprivileges(int to);
        irc::message * err_chanoprisneeded(const str &channel, int to);
        irc::message * err_cantkillserver(int to);
        irc::message * err_restricted(int to);
        irc::message * err_nooperhost(int to);
        irc::message * err_umodeunknownflag(int to);
        irc::message * err_usersdontmatch(int to);
    }

    namespace num
    {
        irc::message * rpl_welcome(const str &userPrefix, int to);
        irc::message * rpl_yourhost(const str &servername, const str &version, int to);
        irc::message * rpl_created(const str &date, int to);
        irc::message * rpl_myinfo(const str &servername, const str &version, const str &availableUserModes, const str &availableChannelModes, int to);
        irc::message * rpl_bounce(const str &servername, const str &port, int to);
        // irc::message * rpl_statslinkinfo(const str &linkname, size_t sendq, size_t sentMessages, size_t sentKbytes, size_t recvMessages, size_t recvKbytes, time_t timeOpen, int to);
        irc::message * rpl_statscommands(const str &command, size_t count, size_t byteCount, size_t remoteCount, int to);
        irc::message * rpl_endofstats(const str &letter, int to);
        irc::message * rpl_umodeis(const str &modes, int to);
        irc::message * rpl_servlist(const str &name, const str &server, const str &mask, const str &type, size_t hopcount, const str &info, int to);
        irc::message * rpl_servlistend(const str &mask, const str &type, int to);
        // irc::message * rpl_statuptime(time_t uptime, int to);
        irc::message * rpl_statoline(const str &hostmask, const str &name, int to);
        irc::message * rpl_away(const str &nickname, const str &reason, int to);
        irc::message * rpl_unaway(int to);
        irc::message * rpl_nowaway(int to);
        irc::message * rpl_list(const str &channel, size_t visible, const str &topic, int to);
        irc::message * rpl_listend(int to);
        irc::message * rpl_channelmodeis(const str &channel, const str &modes, int to);
        irc::message * rpl_notopic(const str &channel, int to);
        irc::message * rpl_topic(const str &channel, const str &topic, int to);
        irc::message * rpl_inviting(const str &channel, const str &nickname, int to);
        irc::message * rpl_invitelist(const str &channel, const str &invitemask, int to);
        irc::message * rpl_endofinvitelist(const str &channel, int to);
        irc::message * rpl_exceptlist(const str &channel, const str &exceptionmask, int to);
        irc::message * rpl_endofexceptlist(const str &channel, int to);
        irc::message * rpl_version(const str &version, const str debuglevel, const str &server, const str &comments, int to);
        irc::message * rpl_namreply(const str &names, int to);
        irc::message * rpl_endofnames(const str &name, int to);
        irc::message * rpl_banlist(const str &channel, const str &banmask, int to);
        irc::message * rpl_endofbanlist(const str &channel, int to);
        irc::message * rpl_motd(const str &text, int to);
        irc::message * rpl_motdstart(const str &servername, int to);
        irc::message * rpl_endofmotd(int to);
        irc::message * rpl_info(const str &info, int to);
        irc::message * rpl_endofinfo(int to);
        irc::message * rpl_youreoper(int to);
        irc::message * rpl_youreservice(const str &servicename, int to);
        irc::message * rpl_time(const str &servername, int to);
        irc::message * rpl_lusersclient(size_t users, size_t services, size_t servers, int to);
        irc::message * rpl_lusersop(size_t nb, int to);
        irc::message * rpl_lusersunknown(size_t nb, int to);
        irc::message * rpl_luserschannels(size_t nb, int to);
        irc::message * rpl_lusersme(size_t nb, int to);
        irc::message * rpl_whoreply(const str &msg, int to);
        irc::message * rpl_endofwho(const str &target, int to);
        irc::message * rpl_whoisuser(const str &nick, const str &user, const str &host, const str &realname, int to);
        irc::message * rpl_whoisserver(const str &nick, const str &servername, const str shortinfo, int to);
        irc::message * rpl_whoisoperator(const str &nick, int to);
        irc::message * rpl_whoisidle(const str &nick, size_t idle, int to);
        irc::message * rpl_whoischannels(const str &nick, const str &status, const str &chan, int to);
        irc::message * rpl_endofwhois(const str &nick, int to);
        irc::message * rpl_whoreply(const str &msg, int to);
        irc::message * rpl_endofwho(const str &target, int to);
        irc::message * rpl_whowasuser(const str &nick, const str &user, const str &host, const str &realname, int to);
        irc::message * rpl_endofwhowas(const str &nick, int to);
        irc::message * rpl_rehashing(const str &filename, int to);
        irc::message * rpl_userhostreply(const str &msg, int to);
    }

    namespace cmd
    {
        irc::message * cmd_ping(const str &nick, int to);
    }  
    
}

#endif