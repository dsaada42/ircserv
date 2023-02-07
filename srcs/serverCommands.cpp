/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverCommands.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 20:03:20 by dylan             #+#    #+#             */
/*   Updated: 2023/02/07 17:41:56 by dsaada           ###   ########.fr       */
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
// ----- INVITE -----   OK
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
             if (channel->get_modes().find("i") != str::npos)
             {
                if (!channel->is_op(sender) && !sender->is_mode('o')) //is sender is not chanop nor servop
                    _messages.push(err::err_chanoprivsneeded(args[1], msg->get_fd()));
                else{
                    if (!channel->is_invit(receiver))
                        channel->add_invit(receiver);
                    _messages.push(rpl::rpl_inviting(channel->get_name(), sender->nickname(), receiver->nickname(), msg->get_fd()));
                    _messages.push(cmd::cmd_invite(user_prefix(sender), receiver->nickname(), channel->get_name(), receiver->fd()));    
                }                    
             }
             else if (!channel->is_user(sender))
                 _messages.push(err::err_notonchannel(args[1], msg->get_fd()));
             else if (channel->is_user(receiver))
                 _messages.push(err::err_useronchannel(args[0], args[1], msg->get_fd()));
             else{
                _messages.push(rpl::rpl_inviting(channel->get_name(), sender->nickname(), receiver->nickname(), msg->get_fd()));
                _messages.push(cmd::cmd_invite(user_prefix(sender), receiver->nickname(), channel->get_name(), receiver->fd()));
             }
         }
    }
// ----- JOIN -----     OK
    void irc::server::ft_join( irc::message * msg) 
	{
		std::vector<str>                    args;
		irc::channel		                *channel;
		irc::user			                *user;
        std::vector<irc::user *>            chan_users;
        std::vector<irc::user *>::iterator  itu;
        irc::message                        *tmp_msg;

		if (!(user = find_user_by_fd(msg->get_fd())))
            return;
		args = ft_split(msg->get_params(), " ");
		if (args.size() < 1)
			 _messages.push(err::err_needmoreparams(msg->get_cmd(), msg->get_fd()));
		else if ((channel = find_channel_by_name(args[0])))
		{
			if (channel->is_ban(user))
			{
				_messages.push(err::err_bannedfromchan(args[0], msg->get_fd()));
				return;
			}
			if (channel->get_modes().find("i") != str::npos && !channel->is_invit(user) && !user->is_mode('o'))
            {
				_messages.push(err::err_inviteonlychan(user->nickname(), args[0], msg->get_fd()));
				return;
			}
			if (channel->get_modes().find("k") != str::npos)
			{
				if (args.size() < 2 || args[1] != channel->get_password())
				{
					_messages.push(err::err_badchannelkey(user->nickname(), args[0], msg->get_fd()));
					return;
				}
			}
			channel->add_user(user);
			chan_users = channel->get_users();
            for (itu = chan_users.begin(); itu != chan_users.end(); itu++)
                _messages.push(cmd::cmd_join(user_prefix(user), channel->get_name(), (*itu)->fd()));
			tmp_msg = new irc::message("", "NAMES", channel->get_name(), "", user->fd());
            ft_names(tmp_msg);
            delete tmp_msg;
            _messages.push(rpl::rpl_topic(user->nickname(), channel->get_name(), channel->get_topic(), msg->get_fd()));
		}
		else
		{
			if (args.size() == 2)
				channel = _channels.insert(std::make_pair(args[0], new irc::channel(args[0], args[1], "", ""))).first->second;
			else
				channel = _channels.insert(std::make_pair(args[0], new irc::channel(args[0], "", "", ""))).first->second;
			channel->add_user(user);
            channel->add_op(user);
			_messages.push(cmd::cmd_join(user_prefix(user), channel->get_name(), msg->get_fd()));
            tmp_msg = new irc::message("", "NAMES", channel->get_name(), "", user->fd());
            ft_names(tmp_msg);
            delete tmp_msg;
			_messages.push(rpl::rpl_topic(user->nickname(), channel->get_name(), channel->get_topic(), msg->get_fd()));
		}
	}
// ----- KICK -----     OK
    void irc::server::ft_kick( irc::message * msg)
	{
		user			                    *sender;
		user			                    *receiver;
		channel			                    *channel;
		std::vector<str>	                args;
        std::vector<irc::user *>            chan_users;
        std::vector<irc::user *>::iterator  itu;

		args = ft_split(msg->get_params(), " ");
		if (args.size() < 2)
			_messages.push(err::err_needmoreparams(msg->get_cmd(), msg->get_fd()));
		else if (!(channel = find_channel_by_name(args[0])))
			_messages.push(err::err_nosuchchannel(args[0], msg->get_fd()));
        else{          
            if (!(sender = find_user_by_fd(msg->get_fd())))
                return;
            if (!channel->is_user(sender))
                _messages.push(err::err_notonchannel(channel->get_name(), msg->get_fd()));
            else if (!channel->is_op(sender) && !sender->is_mode('o'))
                _messages.push(err::err_chanoprivsneeded(channel->get_name(), msg->get_fd()));
            else if (!find_user_by_nick(args[1]))
                _messages.push(err::err_nosuchnick(channel->get_name(), msg->get_fd()));
            else if ((receiver = find_user_by_nick(args[1])))
            {
                if (!channel->is_user(receiver))
                    _messages.push(err::err_usernotinchannel(sender->nickname(), receiver->nickname(), channel->get_name(), msg->get_fd()));
                else{
                    chan_users = channel->get_users();
                    for (itu = chan_users.begin(); itu != chan_users.end(); itu++){
                        if (msg->get_trailing().size() != 0)    
                            _messages.push(cmd::cmd_part(user_prefix(receiver), channel->get_name(), msg->get_trailing(), (*itu)->fd())); 
                        else
                            _messages.push(cmd::cmd_part(user_prefix(receiver), channel->get_name(), "Kicked by oper " + sender->nickname(), (*itu)->fd())); 
                    }
                    channel->delete_user(receiver);
                }
            }
        }
	}
// ----- KILL -----     OK
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
            delete_user(to_kill,"");
        }   
    }
// ----- LIST -----     OK
    void irc::server::ft_list( irc::message * msg ){
        irc::channel *chan;
        irc::user   *usr;
        std::map<str, channel*>::iterator itc;
        std::vector<str>  args;
        std::vector<str>  chans;
        std::vector<str>::iterator it;
        
        args = ft_split(msg->get_params(), " ");
        if (msg->get_params().empty()){
            _messages.push(err::err_needmoreparams(msg->get_cmd(), msg->get_fd()));
            return;
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
                    if (chan->is_mode("p") && !chan->is_user(usr)){
                        _messages.push(rpl::rpl_list(usr->nickname(), chan->get_name(), "Prv", msg->get_fd()));
                    }
                    else{
                        _messages.push(rpl::rpl_list(usr->nickname(), chan->get_name(), chan->get_topic(), msg->get_fd()));
                    }
                }
                _messages.push(rpl::rpl_listend(msg->get_fd()));
            }
        }
        else{//cas ou channels are listed, split args[0] sur ',' and get info for each (only display topic if not private) if args[1] is channel
            if (args[1] != _server_name)
                _messages.push(err::err_nosuchserver(args[0], msg->get_fd()));
            else {        
                chans = ft_split(args[0], ",");
                
                _messages.push(rpl::rpl_liststart(msg->get_fd()));
                for (it = chans.begin(); it != chans.end(); it++){
                    if ((chan = find_channel_by_name(*it)) != NULL){
                        if (chan->is_mode("p") && !chan->is_user(usr)){
                            _messages.push(rpl::rpl_list(usr->nickname(), chan->get_name(), "Prv", msg->get_fd()));
                        }
                        else{
                            _messages.push(rpl::rpl_list(usr->nickname(), chan->get_name(), chan->get_topic(), msg->get_fd()));
                        }
                    }
                }
                _messages.push(rpl::rpl_listend(msg->get_fd()));
            }
        }
    }

    
// ----- MODE -----     OK
    void irc::server::ft_mode(irc::message * msg){
        irc::channel	*channel;
        irc::user   	*current;
        irc::user       *tmp;
        irc::user       *target;
        std::vector<str> args;
        str		m;
        char    c;
        std::vector<user *> chan_users;
        std::vector<user *>::iterator itu; 
        
        current = find_user_by_fd(msg->get_fd());
        if (!current)
            return;
        if ((channel = find_channel_by_name(msg->get_params())) != NULL){
            _messages.push(rpl::rpl_channelmodeis(current->nickname(), channel->get_name(), channel->get_modes(), msg->get_fd()));
            return;
        }
        else if ((tmp = find_user_by_nick(msg->get_params())) != NULL){
            if (tmp == find_user_by_fd(msg->get_fd()))
                _messages.push(rpl::rpl_umodeis(tmp->nickname(), tmp->mode(), tmp->fd()));
            else 
                _messages.push(err::err_usersdontmatch(msg->get_fd()));
            return;
        }
        args = ft_split(msg->get_params(), " ");
        if (args.size() < 2)
            _messages.push(err::err_needmoreparams(msg->get_cmd(), msg->get_fd()));
        else if (check_channel_rules(args[0])){
            if ((channel = find_channel_by_name(args[0]))){
                if (args[1] == "b")
                    return;
                if (!channel->is_op(find_user_by_fd(msg->get_fd())) && !(find_user_by_fd(msg->get_fd()))->is_mode('o'))
                    _messages.push(err::err_chanoprivsneeded(args[0], msg->get_fd()));
                else if (args.size() == 2 && args[1].size() == 2){
                    m = args[1].at(1);
                    if (m == "k"){
                        if (args[1].at(0) == '-'){
                            channel->remove_mode(m);
                            chan_users = channel->get_users();
                            for (itu = chan_users.begin(); itu != chan_users.end(); itu++)
                                _messages.push(cmd::cmd_mode_channel(user_prefix(current), channel->get_name(), "-" + m, (*itu)->fd()));
                        }
                        else if (args[1].at(0) == '+'){
                            _messages.push(err::err_needmoreparams(msg->get_cmd(), msg->get_fd()));
                        }
                        else
                            _messages.push(err::err_unknownmode(current->nickname(), args[1], msg->get_fd()));
                    }
                    else if (m == "p" || m == "t" || m == "i"){
                        if (args[1].at(0) == '+'){
                            channel->add_mode(m);
                            chan_users = channel->get_users();
                            for (itu = chan_users.begin(); itu != chan_users.end(); itu++)
                                _messages.push(cmd::cmd_mode_channel(user_prefix(current), channel->get_name(), "+" + m, (*itu)->fd()));            
                        }
                        else if (args[1].at(0) == '-'){
                            channel->remove_mode(m);
                            chan_users = channel->get_users();
                            for (itu = chan_users.begin(); itu != chan_users.end(); itu++)
                                _messages.push(cmd::cmd_mode_channel(user_prefix(current), channel->get_name(), "-" + m, (*itu)->fd()));
                        }
                    }
                    else if (m == "o"){
                        _messages.push(err::err_needmoreparams(msg->get_cmd(), msg->get_fd()));
                    }
                    else
                        _messages.push(err::err_unknownmode(current->nickname(), args[1], msg->get_fd()));
                }
                else if (args.size() == 3 && args[1].size() == 2){
                    m = args[1].at(1);
                    if (m == "o"){
                        if ((target = find_user_by_nick(args[2])) != NULL){
                            if (channel->is_user(target)){    
                                if (args[1].at(0) == '-'){
                                    if (channel->is_op(target)){
                                        channel->remove_op(target);
                                        chan_users = channel->get_users();
                                        for (itu = chan_users.begin(); itu != chan_users.end(); itu++)
                                            _messages.push(cmd::cmd_mode_channel(user_prefix(current), channel->get_name(), "+o " + target->nickname(), (*itu)->fd()));
                                    }
                                }
                                else if (args[1].at(0) == '+'){
                                    if (!channel->is_op(target)){
                                        channel->add_op(target);
                                        chan_users = channel->get_users();
                                        for (itu = chan_users.begin(); itu != chan_users.end(); itu++)
                                            _messages.push(cmd::cmd_mode_channel(user_prefix(current), channel->get_name(), "+o " + target->nickname(), (*itu)->fd()));
                                    }
                                }
                            }
                            else
                                _messages.push(err::err_usernotinchannel(current->nickname(), target->nickname(), channel->get_name(), msg->get_fd()));
                        }
                        else{
                            _messages.push(err::err_nosuchnick(target->nickname(), msg->get_fd()));
                        }
                    }
                    else if (m == "k"){
                        if (args[1].at(0) == '+'){
                            channel->add_mode(m);
                            channel->set_password(args[2]);
                            chan_users = channel->get_users();
                            for (itu = chan_users.begin(); itu != chan_users.end(); itu++)
                                _messages.push(cmd::cmd_mode_channel(user_prefix(current), channel->get_name(), "+" + m, (*itu)->fd()));
                        }
                        else
                            _messages.push(err::err_unknownmode(current->nickname(), args[1], msg->get_fd()));
                    }
                }
                else
                    _messages.push(err::err_unknownmode(current->nickname(), args[1], msg->get_fd()));
            }
            else
                _messages.push(err::err_nosuchchannel(args[0], msg->get_fd()));
        }
        else{ //USERS
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
                        m = c;
                        if (args[1].at(0) == '+'){
                            if ( c == 'o' ) // ignore +o
                                return;
                            current->change_mode(c, true);
                            _messages.push(cmd::cmd_mode(current->nickname(), "+" + m, current->fd()));
                            return;
                        }        
                        else if (args[1].at(0) == '-'){
                            current->change_mode(c, false);
                            _messages.push(cmd::cmd_mode(current->nickname(), "-" + m, current->fd()));
                            return;
                        }
                    }
                    else
                        _messages.push(err::err_umodeunknownflag(msg->get_fd()));
                }
                else
                    _messages.push(err::err_umodeunknownflag(msg->get_fd()));
            }
        }
    }
// ----- NAMES -----    OK
    void irc::server::ft_names(irc::message * msg){
        std::vector<str>	args;
        std::vector<str>	chans;
        std::vector<user*> 	chan_users;
        irc::channel		*chan;
        irc::user		    *usr;
        str                 users;
        std::map<str, channel*>::iterator itc;
        std::vector<irc::user*>::iterator itu;

        args = ft_split(msg->get_params(), " ");
        if((usr = find_user_by_fd(msg->get_fd())) == NULL)
            return;
        else if (args.empty())//List all visibles channels & users
        {
            for(itc = _channels.begin(); itc != _channels.end(); itc++)
            {
                users = "";
                chan = itc->second;
                if (!chan->is_mode("p") || (chan->is_mode("p") && chan->is_user(usr)))
                {
                    chan_users = chan->get_users();	
                    for (itu = chan_users.begin(); itu != chan_users.end(); itu++)
                    {
                        if (!(*itu)->is_mode('i') || chan->is_op(usr) || usr->is_mode('o')){ //if usr asking is op, can see invisble other users
                            if (users.size() != 0)
                                users += " ";
                            if (chan->is_op(*itu))
                                users += "@" + (*itu)->nickname();      
                            else
                                users += (*itu)->nickname();
                        }
                    }
                    if (users.size() != 0)
                        _messages.push(rpl::rpl_namreply(usr->nickname(), chan->get_name(), users, msg->get_fd()));
                _messages.push(rpl::rpl_endofnames(find_user_by_fd(msg->get_fd())->nickname(), chan->get_name(), msg->get_fd()));
                }
            }
        }
        else if (args.size() == 1)//List users by channel in args
        {
            chans = ft_split(msg->get_params(), ",");

            if (chans.empty())
                return;
            else
            {
                std::vector<str>::iterator it;
                for (it = chans.begin(); it != chans.end(); it++)
                {
                    users = "";
                    if ((chan = find_channel_by_name(*it)))
                    {
                        if (!chan->is_mode("p") || (chan->is_mode("p") && chan->is_user(usr)))
                        {
                            chan_users = chan->get_users();	
                            for (itu = chan_users.begin(); itu != chan_users.end(); itu++)
                            {
                                if (!(*itu)->is_mode('i') || chan->is_op(usr) || usr->is_mode('o')){
                                    if (users.size() != 0)
                                        users += " ";
                                    if (chan->is_op(*itu))
                                        users += "@" + (*itu)->nickname();      
                                    else
                                        users += (*itu)->nickname();
                                }
                            }
                            if (users.size() != 0)
                                _messages.push(rpl::rpl_namreply(usr->nickname(), chan->get_name(), users, msg->get_fd()));
                        _messages.push(rpl::rpl_endofnames(find_user_by_fd(msg->get_fd())->nickname(), chan->get_name(), msg->get_fd()));
                        }
                    }
                }
            }
        }
    }
// ----- NICK -----     OK
    void irc::server::ft_nick(irc::message * msg){
        irc::user       *current;
        
        if (msg->get_params().empty())
            _messages.push(err::err_nonicknamegiven(msg->get_fd()));
        else if (check_nickname_rules(msg->get_params()) == false)
            _messages.push(err::err_erroneusnickname(msg->get_params(), msg->get_fd()));
        else{
            if ((current = find_user_by_fd(msg->get_fd())) == NULL)
                return;
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
            if ((current = find_user_by_fd(msg->get_fd())) == NULL)
                return;
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
        
        if ((current = find_user_by_fd(msg->get_fd())) == NULL)
            return;
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
            chan->delete_user(current);
        }
    }
// ----- PING -----     OK
    void irc::server::ft_ping(irc::message * msg){
        irc::user *usr;

        if ((usr = find_user_by_fd(msg->get_fd())) != NULL){
            _messages.push(cmd::cmd_pong(msg->get_params(), msg->get_fd()));
        }
    }
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

        if ((current = find_user_by_fd(msg->get_fd())) == NULL)
            return;
        for (it = _users.begin(); it != _users.end(); it++){
            if (it->second->nickname() != current->nickname())
                _messages.push(cmd::cmd_quit(user_prefix(current), msg->get_trailing(), it->second->fd()));    
        }
        delete_user(current, "");
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
                if (chan->get_modes().find('t') != str::npos && !chan->is_op(usr))
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
        if ((current = find_user_by_fd(msg->get_fd())) == NULL)
            return;
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
// ----- WHO ------     OK
    void irc::server::ft_who(irc::message * msg){
        std::vector<str> args;
        irc::user        *current;
        irc::user        *sender;
        irc::channel     *chan;
        int              found;
        std::map<str, channel*>::iterator itc;
        
        found = 0;
        if ((sender = find_user_by_fd(msg->get_fd())) == NULL)
            return;
        if (msg->get_params().empty())
            _messages.push(err::err_nonicknamegiven(msg->get_fd()));
        else if ((current = find_user_by_fd(msg->get_fd())) != NULL){
            args = ft_split(msg->get_params(), " ");
            if (args.size() == 1){
                if ((current = find_user_by_nick(args[0])) != NULL){
                    if (!current->is_mode('i')){
                        for (itc = _channels.begin(); itc != _channels.end(); itc++){
                            chan = itc->second;
                            if (chan->is_op(current)){
                                found ++;
                                _messages.push(rpl::rpl_whoreply(sender->nickname(), chan->get_name() + " ~" + current->username() + " " + _server_name + " " + _server_name + " " + current->nickname() + " H@", "0 " + current->fullname() , msg->get_fd())); 
                            }
                        }
                        if (found == 0)
                            _messages.push(rpl::rpl_whoreply(sender->nickname(), "* ~" + current->username() + " " + _server_name + " " + _server_name + " " + current->nickname() + " H", "0 " + current->fullname(), msg->get_fd() )); 
                    }
                }
            }
            else if (args.size() == 2 && args[1] == "o"){
                if ((current = find_user_by_nick(args[0])) != NULL){
                    if (!current->is_mode('i')){    
                        for (itc = _channels.begin(); itc != _channels.end(); itc++){
                            chan = itc->second;
                            if (chan->is_op(current)){
                                _messages.push(rpl::rpl_whoreply(sender->nickname(), chan->get_name() + " ~" + current->username() + " " + _server_name + " " + _server_name + " " + current->nickname() + " H@", "0 " + current->fullname() , msg->get_fd())); 
                            }
                        }
                    }
                }
            }
            _messages.push(rpl::rpl_endofwho(sender->nickname(), args[0], msg->get_fd()));
        }
    }
// ----- WHOIS -----    OK
    int irc::server::who_one_nick(irc::user *sender, str nick){
        irc::user *target;
        std::map<str, channel*>::iterator it;
        str chans;
        irc::channel    *chan;
        int  found = 0;
        
        chans = "";
        if (!(target = find_user_by_nick(nick)))//USER NOT FOUND
            return (FAILURE);
        _messages.push(rpl::rpl_whoisuser(sender->nickname(), target->nickname(), target->username(), _server_name, target->fullname(), sender->fd()));
        for (it = _channels.begin(); it != _channels.end(); it++){
            chan = it->second;
            if (chan->is_op(target)){
                found++;
                if (chans.size() != 0)
                    chans += " ";
                chans += "@" + chan->get_name();
            }
            else if (chan->is_user(target)){
                found++;
                if (chans.size() != 0)
                    chans += " ";
                chans += chan->get_name();
            }
        }
        if (found != 0)
            _messages.push(rpl::rpl_whoischannels(sender->nickname(), target->nickname(), chans, sender->fd() ));
        _messages.push(rpl::rpl_whoisserver(sender->nickname(), target->nickname(), _server_name, "The one and only", sender->fd()));
        _messages.push(rpl::rpl_whoisidle(sender->nickname(), target->nickname(), SSTR((get_time_ms() - target->creation_time())/1000), SSTR(target->creation_time()), sender->fd()));
        return (SUCCESS);
    }
    void irc::server::ft_whois(irc::message * msg)
    {
        irc::user               *sender;
        std::vector<str>        args;
        std::vector<str>        nicks;
        std::vector<str>::iterator itn;
        

        if (msg->get_params().empty())
            _messages.push(err::err_nonicknamegiven(msg->get_fd()));
        if ((sender = find_user_by_fd(msg->get_fd())) == NULL)
            return;
        args = ft_split(msg->get_params(), " ");
        if (args.size() == 1)//ONE ARG
        {
            nicks = ft_split(args[0], ",");
            for (itn = nicks.begin(); itn != nicks.end(); itn++){ // parcourir tous les users et les traiter un par un
                if (who_one_nick(sender, (*itn)) == FAILURE)
                    _messages.push(err::err_nosuchnick((*itn), msg->get_fd()));            
            }
            _messages.push(rpl::rpl_endofwhois(sender->nickname(), args[0], sender->fd()));
        }
        else if (args.size() == 2)//TWO OR MORE ARGS
        {
            if (args[0] != _server_name)
                _messages.push(err::err_nosuchserver(args[0], msg->get_fd()));
            else{
                nicks = ft_split(args[1], ",");
                for (itn = nicks.begin(); itn != nicks.end(); itn++){ // parcourir tous les users et les traiter un par un
                    if (who_one_nick(sender, (*itn)) == FAILURE)
                        _messages.push(err::err_nosuchnick((*itn), msg->get_fd()));            
                }
                _messages.push(rpl::rpl_endofwhois(sender->nickname(), args[1], sender->fd()));
            }
        }
    }
// ----- WHOWAS -----   OK
    void irc::server::ft_whowas(irc::message * msg){
        irc::user *usr;
        irc::user *sender;
        int found;
        std::map<int, irc::user*>::iterator itu;

        if ((sender = find_user_by_fd(msg->get_fd())) == NULL)
            return;
        if (msg->get_params().empty())
            _messages.push(err::err_nonicknamegiven(msg->get_fd()));
        else{
            found = 0;
            for (itu = _users.begin(); itu != _users.end(); itu++){
                usr = itu->second;
                if (usr->oldnick() == msg->get_params()){
                    _messages.push(rpl::rpl_whowasuser(sender->nickname(), msg->get_params(), usr->username(), _server_name, usr->fullname(), msg->get_fd()));
                    _messages.push(rpl::rpl_whoisserver(sender->nickname(), msg->get_params(), _server_name, "The one and only", msg->get_fd()));
                    found++;
                }
            }
            if (found == 0)
                _messages.push(err::err_wasnosuchnick(sender->nickname(), msg->get_params(), msg->get_fd()));
            _messages.push(rpl::rpl_endofwhowas(sender->nickname(), msg->get_params(), sender->fd()));
        }
    }
// ----- USERS -----    OK
    void irc::server::ft_users(irc::message * msg){ _messages.push(err::err_usersdisabled(msg->get_fd())); }
// ----- SUMMON -----   OK
    void irc::server::ft_summon(irc::message * msg){ _messages.push(err::err_summondisabled(msg->get_fd())); }
