/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 14:20:53 by dsaada            #+#    #+#             */
/*   Updated: 2023/02/03 15:08:40 by dsaada           ###   ########.fr       */
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

//SETTERS

void				irc::channel::set_topic(const str &topic){ _topic = topic; }

 //CHECKING
bool			irc::channel::is_empty( void ){
	return (_users.size() == 0);
}

bool			irc::channel::is_op(user *obj)
{
	std::vector<irc::user*>::iterator it;

	//if user mode +o return true
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
//REMOVING
void			irc::channel::remove_user(user *obj)
{
	std::vector<irc::user*>::iterator it;

	for (it = _users.begin(); it != _users.end(); it++)
	{
		if (*it == obj)
		{
			_users.erase(it);
			break;
		}
	}
}

void			irc::channel::remove_op(user *obj)
{
	std::vector<irc::user*>::iterator it;

	for (it = _op.begin(); it != _op.end(); it++)
	{
		if (*it == obj)
		{
			_op.erase(it);
			break;
		}
	}
}

void			irc::channel::remove_invit(user *obj)
{
	std::vector<irc::user*>::iterator it;

	for (it = _invit.begin(); it != _invit.end(); it++)
	{
		if (*it == obj)
		{
			_invit.erase(it);
			break;
		}
	}
}

void			irc::channel::remove_ban(user *obj)
{
	std::vector<irc::user*>::iterator it;

	for (it = _ban.begin(); it != _ban.end(); it++)
	{
		if (*it == obj)
		{
			_ban.erase(it);
			break;
		}
	}
}


