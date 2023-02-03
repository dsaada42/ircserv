/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverCommands.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 20:03:20 by dylan             #+#    #+#             */
/*   Updated: 2023/02/04 00:25:52 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

//===========================================================================================
//=                                                                                         =
//=                                 RECEIVED COMMANDS                                       =
//=                                                                                         =
//===========================================================================================

// ----- ADMIN -----    OK
    void irc::server::ft_admin( irc::message * msg ){
        if (msg->get_params().empty())
            _messages.push(err::err_noadmininfo(msg->get_fd()));
        else if (msg->get_params() != SERVER_NAME)
            _messages.push(err::err_nosuchserver(msg->get_params(), msg->get_fd()));
        else
            _messages.push(err::err_noadmininfo(msg->get_fd()));
    }
// ----- CAP -----      OK
    void irc::server::ft_cap( irc::message * msg ){(void)msg;}//ignore CAP messages
// ----- INFO -----     OK
    void irc::server::ft_info( irc::message * msg ){ //OK
        if (!msg->get_params().empty() && msg->get_params() != _server_name){
            _messages.push(err::err_nosuchserver(msg->get_params(), msg->get_fd()));
        }
        else{
            _messages.push(rpl::rpl_info("Server name: " + _server_name, msg->get_fd()));
            _messages.push(rpl::rpl_info("Maintainers: Dylan and Ryad", msg->get_fd()));
            _messages.push(rpl::rpl_info("Creation date: " + _creation_date, msg->get_fd()));
            _messages.push(rpl::rpl_info("Number of connected users: " + SSTR(_users.size()), msg->get_fd()));
            _messages.push(rpl::rpl_info("Number of channels: " + SSTR(_channels.size()), msg->get_fd()));
            _messages.push(rpl::rpl_endofinfo(msg->get_fd()));
        } 
    }
// ----- INVITE -----   OK -> NEED TESTS
    void irc::server::ft_invite( irc::message * msg )
	{
        channel			    *channel;
        user			    *sender;
        user			    *receiver;
	    str			        modes;
        std::vector<str> 	args;

         if ((sender = find_user_by_fd(msg->get_fd())) == NULL)
            return;
         args = ft_split(msg->get_params(), " ");

         if (args.size() < 2)
             _messages.push(err::err_needmoreparams(msg->get_cmd(), msg->get_fd()));
         else if (!(receiver = find_user_by_nick(args[0])))
             _messages.push(err::err_nosuchnick(args[0], msg->get_fd()));
         else if (!(channel = find_channel_by_name(args[1])))
             _messages.push(err::err_nosuchchannel(args[1], msg->get_fd()));
         else//Channel exists
         {
             if (channel->get_modes().find("i") != str::npos) //============ NEED TESTS ============
             {
                if (!channel->is_op(sender))
                    _messages.push(err::err_chanoprivsneeded(args[1], msg->get_fd()));
                else{
                    if (!channel->is_invit(receiver))
                        channel->add_invit(receiver);
                    _messages.push(rpl::rpl_inviting(channel->get_name(), sender->nickname(), receiver->nickname(), msg->get_fd()));
                    _messages.push(cmd::cmd_invite(user_prefix(sender), receiver->nickname(), channel->get_name(), receiver->fd()));    
                }                    
             }//=======================================================================================
             else if (!channel->is_user(sender))//If sender is not on channel
                 _messages.push(err::err_notonchannel(args[1], msg->get_fd()));
             else if (channel->is_user(receiver))//If receiver is already on channel
                 _messages.push(err::err_useronchannel(args[0], args[1], msg->get_fd()));
             else{
                _messages.push(rpl::rpl_inviting(channel->get_name(), sender->nickname(), receiver->nickname(), msg->get_fd()));
                _messages.push(cmd::cmd_invite(user_prefix(sender), receiver->nickname(), channel->get_name(), receiver->fd()));
             }
         }
    }
// ----- JOIN -----     OK? -> NEED TESTS
    void irc::server::ft_join( irc::message * msg) 
	{
		std::vector<str>                args;
		irc::channel		            *channel;
		irc::user			            *user;
        std::vector<irc::user *>             chan_users;
        std::vector<irc::user *>::iterator   itu;

		if (!(user = find_user_by_fd(msg->get_fd())))
            return;
		args = ft_split(msg->get_params(), " ");
		if (args.size() < 1 || args.size() > 2)//Not enough args
			 _messages.push(err::err_needmoreparams(msg->get_cmd(), msg->get_fd()));
		else if ((channel = find_channel_by_name(args[0])))//If channel exists
		{//============================ NEEDS TESTS =========================================================
			if (channel->is_ban(user))//If user is banned
			{
				_messages.push(err::err_bannedfromchan(args[0], msg->get_fd()));
				return;
			}
			if (channel->get_modes().find("i") != str::npos) //If channel is invtationnal
			{
				if (!channel->is_invit(user))//If user is not on invit list
				{
					_messages.push(err::err_inviteonlychan(args[0], msg->get_fd()));
					return;
				}
			}
			if (channel->get_modes().find("k") != str::npos)//If channel has a password
			{
				if (args.size() < 2 || args[1] != channel->get_password())//Bad password
				{
					_messages.push(err::err_badchannelkey(args[0], msg->get_fd()));
					return;
				}
			}//=================================================================================================
			channel->add_user(user);
			chan_users = channel->get_users();
            for (itu = chan_users.begin(); itu != chan_users.end(); itu++)
                _messages.push(cmd::cmd_join(user_prefix(user), channel->get_name(), (*itu)->fd()));
			_messages.push(rpl::rpl_topic(user->nickname(), channel->get_name(), channel->get_topic(), msg->get_fd()));
		}
		else//If channel doesn't exist
		{//We create a new channel with or without password
			if (args.size() == 2)
				channel = _channels.insert(std::make_pair(args[0], new irc::channel(args[0], args[1], "", ""))).first->second;
			else if (args.size() == 1)
				channel = _channels.insert(std::make_pair(args[0], new irc::channel(args[0], "", "Topitopic", ""))).first->second;
			channel->add_user(user);
			_messages.push(cmd::cmd_join(user_prefix(user), channel->get_name(), msg->get_fd()));
			_messages.push(rpl::rpl_topic(user->nickname(), channel->get_name(), channel->get_topic(), msg->get_fd()));

		}
	}
// ----- KICK -----
    void irc::server::ft_kick( irc::message * msg)
	{
		user			*sender;
		user			*receiver;
		channel			*channel;
		std::vector<str>	args;

		args = ft_split(msg->get_params(), " ");
		if (args.size() < 2)//Not enough params
		{
			_messages.push(err::err_needmoreparams(msg->get_cmd(), msg->get_fd()));
			return;
		}
		if (!find_channel_by_name(args[0]))//If channel doesn't exist
		{
			_messages.push(err::err_nosuchchannel(args[0], msg->get_fd()));
			return;
		}
		channel = find_channel_by_name(args[0]);
		sender = find_user_by_fd(msg->get_fd());
		if (!channel->is_op(sender))//If sender is not op ================== CHECK IF SERVER OP
		{
			_messages.push(err::err_chanoprivsneeded(args[0], msg->get_fd()));
			return;
		}
		if (!find_user_by_nick(args[1]))//If receiver is not on server
		{
			_messages.push(err::err_notonchannel(args[0], msg->get_fd()));
			return;
		}
		else if ((receiver = find_user_by_nick(args[1])))//If receiver is in server
		{
			if (!channel->is_user(receiver))//If receiver is not in channel
			{
				_messages.push(err::err_notonchannel(args[0], msg->get_fd()));
				return;
			}
			channel->remove_user(receiver);
			//Reponse client ?
		}
	}
// ----- KILL ----- OK
    void irc::server::ft_kill( irc::message * msg ){
        irc::user   *usr;
        irc::user   *to_kill;
        std::map<int, user*>::iterator itu;
        
        if ((usr = find_user_by_fd(msg->get_fd())) == NULL)
            return;
        if (!usr->is_mode('o')){
            _messages.push(err::err_noprivileges(msg->get_fd()));
        }
        else if (msg->get_params().empty()){
            _messages.push(err::err_needmoreparams(msg->get_cmd(), msg->get_fd()));
        }
        else if ((to_kill = find_user_by_nick(msg->get_params())) == NULL){
            _messages.push(err::err_nosuchnick(msg->get_params(), msg->get_fd()));
        }
        else{
            for (itu = _users.begin(); itu != _users.end(); itu++){
                if (msg->get_trailing().empty())
                    _messages.push(cmd::cmd_quit(user_prefix(to_kill), "Got killed by oper " + usr->nickname(), itu->second->fd()));
                else
                    _messages.push(cmd::cmd_quit(user_prefix(to_kill), msg->get_trailing(), itu->second->fd()));
            }
            delete_user(to_kill);
        }   
    }
// ----- LIST ----- //A COMPLETER POUR LE CAS DE LA CHANNEL LIST
    void irc::server::ft_list( irc::message * msg ){
        irc::channel *chan;
        irc::user   *usr;
        std::map<str, channel*>::iterator itc;
        std::vector<str>  args;
        std::vector<str>  chans;
        
        args = ft_split(msg->get_params(), " ");
        if (msg->get_params().empty()){
            _messages.push(err::err_needmoreparams(msg->get_cmd(), msg->get_fd()));
        }
        if ((usr = find_user_by_fd(msg->get_fd())) == NULL)
            return;
        if (args.size() == 1){
            if (args[0] != _server_name)
                _messages.push(err::err_nosuchserver(args[0], msg->get_fd()));
            else{
                _messages.push(rpl::rpl_liststart(msg->get_fd()));
                for(itc = _channels.begin(); itc != _channels.end(); itc++){
                    chan = itc->second;
                    // if (chan->is_mode("p") && !chan->is_user(usr)){
                    //     _messages.push(rpl::rpl_list(usr->nickname(), chan->get_name(), "Prv", msg->get_fd()));
                    // }
                    // else{
                        _messages.push(rpl::rpl_list(usr->nickname(), chan->get_name(), chan->get_topic(), msg->get_fd()));
                    // }
                }
                _messages.push(rpl::rpl_listend(msg->get_fd()));
            }
        }
        else{//cas ou channels are listed, split args[0] sur ',' and get info for each (only display topic if not private) if args[1] is channel
            
        }
    }
// ----- MODE -----
    void irc::server::ft_mode(irc::message * msg){
        irc::user   *current;
        std::vector<str> args;
        char        c;
        
        //channels ?
        
        //users    
        args = ft_split(msg->get_params(), " ");
        if (args.size() < 2){
            _messages.push(err::err_needmoreparams(msg->get_cmd(), msg->get_fd()));
            return;
        }
        current = find_user_by_nick(args[0]);
        if (!current)
            _messages.push(err::err_nosuchnick(args[0], msg->get_fd()));
        else if (find_user_by_fd(msg->get_fd()) != current){
            _messages.push(err::err_usersdontmatch(msg->get_fd()));
        }
        else{
            if (args[1].size() == 2){
                c = args[1].at(1);
                if (c == 'i' || c == 'o' || c == 's'){
                    if (args[1].at(0) == '+'){
                        if ( c == 'o' ) // ignore +o
                            return;
                        current->change_mode(c, true);
                        _messages.push(rpl::rpl_umodeis(current->nickname(), current->mode(), current->fd()));
                        return;
                    }        
                    else if (args[1].at(0) == '-'){
                        current->change_mode(c, false);
                        _messages.push(rpl::rpl_umodeis(current->nickname(), current->mode(), current->fd()));
                        return;
                    }
                }
            }
            else
                _messages.push(err::err_umodeunknownflag(msg->get_fd()));
        }
    }
// ----- NAMES -----
    void irc::server::ft_names(irc::message * msg){
        //lists all nicknames visible on all visible channels (if a channel is private or secret, it won't appear unless the user is part of it)
        //params can be a list of channels (will get an answer only if channel is visible)
        //RPL_NAMREPLY
        //RPL_ENDOFNAMES 
        (void)msg;
    }
// ----- NICK -----     OK
    void irc::server::ft_nick(irc::message * msg){
        irc::user       *current;
        
        if (msg->get_params().empty())
            _messages.push(err::err_nonicknamegiven(msg->get_fd()));
        else if (check_nickname_rules(msg->get_params()) == false)
            _messages.push(err::err_erroneusnickname(msg->get_params(), msg->get_fd()));
        else{
            current = find_user_by_fd(msg->get_fd());
            if (is_a_nickname(msg->get_params()) == false){
                if (current->connected())
                    _messages.push(cmd::cmd_nick(user_prefix(current), msg->get_params(), msg->get_fd()));
                current->set_nickname(msg->get_params());
            }
            else if (current->nickname() != msg->get_params())
                _messages.push(err::err_nicknameinuse(msg->get_params(), msg->get_fd()));
        }
    }
// ----- NOTICE -----   OK
    void irc::server::ft_notice(irc::message * msg){
        irc::user       *current;
        irc::channel    *chan;
        irc::user       *from;
        std::vector<irc::user *>             chan_users;
        std::vector<irc::user *>::iterator   itu;
        
        if (!msg->get_params().empty() && !msg->get_trailing().empty()){
            if ((from = find_user_by_fd(msg->get_fd())) == NULL)
                return;
            if ((chan = find_channel_by_name(msg->get_params())) != NULL){
                chan_users = chan->get_users();
                for (itu = chan_users.begin(); itu != chan_users.end(); itu++){
                    if (from != (*itu))    
                        _messages.push(cmd::cmd_notice(user_prefix(from), chan->get_name(), msg->get_trailing(), (*itu)->fd()));
                }
            }
            else if ((current = find_user_by_nick(msg->get_params())) != NULL){
                _messages.push(cmd::cmd_notice(user_prefix(from), current->nickname(), msg->get_trailing(), current->fd()));
            }
        }
    }
// ----- OPER -----     OK
    void irc::server::ft_oper(irc::message * msg){
        std::vector<str>    args;
        irc::user           *current;
        std::map<int, irc::user*>::iterator it;
        
        args = ft_split(msg->get_params(), " ");
        if (args.size() < 2){
            _messages.push(err::err_needmoreparams(msg->get_cmd(), msg->get_fd()));
            return;
        }
        if ((current = find_user_by_nick(args[0])) != NULL){
            if (current->fd() == msg->get_fd()){
                if (args[1] == OPER_PASSWORD){
                    _messages.push(rpl::rpl_youreoper(current->nickname(), msg->get_fd()));
                    current->change_mode('o', true);
                    for (it = _users.begin(); it != _users.end(); it++){
                        _messages.push(cmd::cmd_mode(current->nickname(), "+o", it->second->fd()));    
                    }
                }
                else{
                    _messages.push(err::err_passwdmissmatch(msg->get_fd()));
                }
            }
        }
        else{
            _messages.push(err::err_nooperhost(msg->get_fd()));
        }
    }
// ----- PASS -----     OK
    void irc::server::ft_pass(irc::message * msg){
        irc::user * current;

        if (msg->get_params().empty())
            _messages.push(err::err_needmoreparams(msg->get_cmd(), msg->get_fd()));   
        else{
            current = find_user_by_fd(msg->get_fd());
            if (current->connected()){
                _messages.push(err::err_alreadyregistred(msg->get_fd()));
            }
            else{
                if (msg->get_params() == _pass)
                    current->set_pass(true);
                else
                    current->set_pass(false);
            }
        }
    }
// ----- PART -----     OK
    void irc::server::ft_part(irc::message * msg){
        irc::channel    *chan;
        irc::user       *current;
        std::vector<irc::user *>             chan_users;
        std::vector<irc::user *>::iterator   itu;
        
        current = find_user_by_fd(msg->get_fd());
        if (msg->get_params().empty()){ //no channel given
            _messages.push(err::err_needmoreparams(msg->get_cmd(), msg->get_fd()));
        } //channel doesn't exist
        else if ((chan = find_channel_by_name(msg->get_params())) == NULL){
            _messages.push(err::err_nosuchchannel(msg->get_params(), msg->get_fd()));
        }
        else if (!chan->is_user(current)){// user is not on channel
            _messages.push(err::err_notonchannel(current->nickname(), msg->get_fd()));
        }
        else{//user is on channel
            chan_users = chan->get_users();
            for (itu = chan_users.begin(); itu != chan_users.end(); itu++){
                _messages.push(cmd::cmd_part(user_prefix(current), chan->get_name(), msg->get_trailing(), (*itu)->fd()));
            }
            chan->remove_user(current);
        }
    }
// ----- PING -----     OK
    void irc::server::ft_ping(irc::message * msg){(void)msg;}// server will ignore message but listen to the ping to refresh inactivity timestamp of sending user
// ----- PONG -----     OK
    void irc::server::ft_pong(irc::message * msg){
        irc::user   *current;
        
        if (msg->get_params().empty()){
            _messages.push(err::err_noorigin(msg->get_fd()));
        }
        else if (msg->get_params() != SERVER_NAME){
            _messages.push(err::err_nosuchserver(msg->get_params(), msg->get_fd()));
        }
        else {
            current = find_user_by_fd(msg->get_fd());
            if (current)
                current->set_ping(false);
        }
    }
// ----- PRIVMSG -----  OK
    void irc::server::ft_privmsg(irc::message * msg){
        irc::user       *current;
        irc::channel    *chan;
        irc::user       *from;
        std::vector<user *> chan_users;
        std::vector<user *>::iterator itu;

        if (msg->get_params().empty()){
            _messages.push(err::err_norecipient(msg->get_cmd(), msg->get_fd()));
        }
        else if (msg->get_trailing().empty()){
            _messages.push(err::err_notexttosend(msg->get_fd()));
        }
        else{
            if ((from = find_user_by_fd(msg->get_fd())) == NULL)
                return;
            if ((chan = find_channel_by_name(msg->get_params())) != NULL){
                chan_users = chan->get_users();
                for (itu = chan_users.begin(); itu != chan_users.end(); itu++){
                    if (*itu != from)
                        _messages.push(cmd::cmd_privmsg(user_prefix(from), chan->get_name(), msg->get_trailing(), (*itu)->fd()));
                }
            }
            else if ((current = find_user_by_nick(msg->get_params())) != NULL){
                _messages.push(cmd::cmd_privmsg(user_prefix(from), current->nickname(), msg->get_trailing(), current->fd()));
            }
            else
                _messages.push(err::err_nosuchnick(msg->get_params(), msg->get_fd()));
        }
    }
// ----- QUIT -----     OK
    void irc::server::ft_quit(irc::message * msg){
        irc::user   *current;
        std::map<int, irc::user*>::iterator it;

        current = find_user_by_fd(msg->get_fd());
        for (it = _users.begin(); it != _users.end(); it++){
            if (it->second->nickname() != current->nickname())
                _messages.push(cmd::cmd_quit(user_prefix(current), msg->get_trailing(), it->second->fd()));    
        }
        delete_user(current);
    }
// ----- STATS -----    OK
    void irc::server::ft_stats(irc::message * msg){
        std::vector<str> args;
        std::map<int, irc::user*>::iterator it;
        std::map<str, void (server::*)(irc::message *)>::iterator itf;
        irc::user                           *current;
        unsigned long                       uptime;

        current = find_user_by_fd(msg->get_fd());
        if (current){
            args = ft_split(msg->get_params(), " ");
            if (args.size() == 2){
                if (args[1] != _server_name){
                    _messages.push(err::err_nosuchserver(args[1], msg->get_fd()));
                    return;
                }
                if (args[0] == "l"){
                    for (it = _users.begin(); it != _users.end(); it++){
                        _messages.push(rpl::rpl_statslinkinfo( current->nickname(), it->second->connection_stats(), msg->get_fd()));
                    }
                    _messages.push(rpl::rpl_endofstats(current->nickname(), "l", msg->get_fd()));    
                }
                else if (args[0] == "u"){
                    uptime = get_time_ms() - _creation_time;
                    _messages.push(rpl::rpl_statsuptime(current->nickname(), ft_ms_to_duration(uptime), msg->get_fd()));
                    _messages.push(rpl::rpl_endofstats(current->nickname(), "u", msg->get_fd()));    
                }
                else if (args[0] == "m"){
                    for (itf = _cmds.begin(); itf != _cmds.end(); itf++){
                        _messages.push(rpl::rpl_statscommands(current->nickname(), itf->first, msg->get_fd()));
                    }
                    _messages.push(rpl::rpl_endofstats(current->nickname(), "m", msg->get_fd()));
                }
            }
            else{
                _messages.push(rpl::rpl_endofstats(current->nickname(), msg->get_params(), msg->get_fd()));
            }
        }
    }
// ----- TIME -----     OK
    void irc::server::ft_time(irc::message * msg){
        if (!msg->get_params().empty() && msg->get_params() != SERVER_NAME){
            _messages.push(err::err_nosuchserver(msg->get_params(), msg->get_fd()));
        }
        else
            _messages.push(rpl::rpl_time(SERVER_NAME, msg->get_fd()));
    }
// ----- TOPIC -----    OK
    void irc::server::ft_topic(irc::message * msg){
        irc::channel     *chan;
        irc::user        *usr;
        std::vector<irc::user *>             chan_users;
        std::vector<irc::user *>::iterator   itu;
        
        if (msg->get_params().empty())
            _messages.push(err::err_needmoreparams(msg->get_cmd(), msg->get_fd()));
        else{
            if ((usr = find_user_by_fd(msg->get_fd())) == NULL)
                return;
            if ((chan = find_channel_by_name(msg->get_params())) == NULL){
                _messages.push(err::err_nosuchchannel(msg->get_params(), msg->get_fd()));
            }
            else if (!chan->is_user(usr)){
                _messages.push(err::err_notonchannel(usr->nickname(), msg->get_fd()));
            }
            else if (msg->get_trailing().size() > 0){
                if (chan->get_modes().find('t') != str::npos && !chan->is_op(usr)) // si le mode t est active et l'user n'est pas op
                    _messages.push(err::err_chanoprivsneeded(chan->get_name(), msg->get_fd()));
                else{
                    chan->set_topic(msg->get_trailing());
                    chan_users = chan->get_users();
                    for (itu = chan_users.begin(); itu != chan_users.end(); itu++){
                        _messages.push(rpl::rpl_topic((*itu)->nickname(), chan->get_name(), chan->get_topic(), (*itu)->fd()));
                    }
                }
            }
            else{
                if (chan->get_topic().size() == 0)
                    _messages.push(rpl::rpl_notopic(usr->nickname(), chan->get_name(), usr->fd()));
                else
                    _messages.push(rpl::rpl_topic(usr->nickname(), chan->get_name(), chan->get_topic(), usr->fd()));
            }
        }
    } 
// ----- USER -----     OK
    void irc::server::ft_user(irc::message * msg){
        irc::user * current;
        std::vector<str> args;

        if (msg->get_params().empty() || msg->get_trailing().empty()){
            _messages.push(err::err_needmoreparams(msg->get_cmd(), msg->get_fd()));   
            return;
        }
        args = ft_split(msg->get_params(), " ");
        if (args.size() < 3){
            _messages.push(err::err_needmoreparams(msg->get_cmd(), msg->get_fd()));
            return;
        }
        current = find_user_by_fd(msg->get_fd());
        if (current->username().size() != 0){
            _messages.push(err::err_alreadyregistred(msg->get_fd()));
            return;
        }
        if (current){
            current->set_username(args[0]);
            current->set_fullname(msg->get_trailing());
        }
    }
// ----- VERSION -----  OK
    void irc::server::ft_version(irc::message * msg){
        irc::user *current;

        current = find_user_by_fd(msg->get_fd());
        if (!current)
            return;
        if (msg->get_params().empty() || msg->get_params() == SERVER_NAME)
            _messages.push(rpl::rpl_version(current->nickname(), SERVER_VERSION, "", SERVER_NAME , "The one and only", msg->get_fd()));
        else
            _messages.push(err::err_nosuchserver(msg->get_params(), msg->get_fd()));
    }
// ----- WHO ------     
    void irc::server::ft_who(irc::message * msg){
        std::vector<str> args;
        irc::user        *current;
        
        if (msg->get_params().empty())
            _messages.push(err::err_nonicknamegiven(msg->get_fd()));
        else if ((current = find_user_by_fd(msg->get_fd())) != NULL){
            args = ft_split(msg->get_params(), " ");
            if (args.size() == 1){
                //search what is given as param
                //priority to search for channels, if name given isn't channel , search for everything
            }
            else if (args.size() == 2 && args[1] == "o"){
                //search for param as operator
            }
            _messages.push(rpl::rpl_endofwho(current->nickname(), args[0], msg->get_fd()));
        }
    }
// ----- WHOIS -----
    void irc::server::ft_whois(irc::message * msg){(void)msg;}
// ----- WHOWAS -----
    void irc::server::ft_whowas(irc::message * msg){(void)msg;}
// ----- USERS -----    OK
    void irc::server::ft_users(irc::message * msg){ _messages.push(err::err_usersdisabled(msg->get_fd())); }
// ----- SUMMON -----   OK
    void irc::server::ft_summon(irc::message * msg){ _messages.push(err::err_summondisabled(msg->get_fd())); }
