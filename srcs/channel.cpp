/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 14:20:53 by dsaada            #+#    #+#             */
/*   Updated: 2023/02/02 11:59:17 by mlaouedj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

#include "user.hpp"

 //CONSTRUCTOR& DESTRUCTOR
 irc::channel::channel(str name, str pass, str topic, str modes): _name(name), _password(pass), _topic(topic), _modes(modes) {}

 irc::channel::~channel(void) {}

 //GETTERS
std::vector<irc::user*>	irc::channel::get_users(void) const { return _users; }

std::vector<irc::user*>	irc::channel::get_op(void) const { return _op; }
 
std::vector<irc::user*>	irc::channel::get_invit(void) const { return _invit; }

std::vector<irc::user*> irc::channel::get_ban(void) const { return _ban; }

const str			&irc::channel::get_name(void) const { return _name; }

const str			&irc::channel::get_password(void) const { return _password; }

const str			&irc::channel::get_topic(void) const { return _topic; }

const str			&irc::channel::get_modes(void) const { return _modes; }

 //CHECKING
bool			irc::channel::is_op(user *obj)
{
	std::vector<irc::user*>::iterator it;

	for (it = _op.begin(); it != _op.end(); it++)
	{
		if (*it == obj)
			return true;
	}
	return false;
}
bool			irc::channel::is_user(user *obj)
{
	std::vector<irc::user*>::iterator it;

	for (it = _users.begin(); it != _users.end(); it++)
	{
 		if (*it == obj)
			return true;
	}
	return false;
}
bool			irc::channel::is_invit(user*obj)
{
	std::vector<irc::user*>::iterator it;

	for (it = _invit.begin(); it != _invit.end(); it++)
	{
		if (*it == obj)
			return true;
	}
	return false;
}
bool			irc::channel::is_ban(user *obj)
{
	std::vector<irc::user*>::iterator it;

	for (it = _ban.begin(); it != _ban.end(); it++)
	{
		if (*it == obj)
			return true;
	}
	return false;
}
//ADDING
void			irc::channel::add_user(user *obj) { _users.push_back(obj); }
void			irc::channel::add_op(user *obj) { _op.push_back(obj); }
void			irc::channel::add_invit(user *obj) { _invit.push_back(obj); }
