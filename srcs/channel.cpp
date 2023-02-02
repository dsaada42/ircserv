/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 14:20:53 by dsaada            #+#    #+#             */
/*   Updated: 2023/02/02 08:10:27 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

#include "user.hpp"

 //CONSTRUCTOR& DESTRUCTOR
 irc::channel::channel(str name, str topic, str modes): _name(name), _topic(topic), _modes(modes) {}

 irc::channel::~channel(void) {}

 //GETTERS
 std::vector<irc::user>	irc::channel::get_op(void) const { return _op; }

 str			        irc::channel::get_name(void) const { return _name; }

 str			        irc::channel::get_topic(void) const { return _topic; }

 str			        irc::channel::get_modes(void) const { return _modes; }

 //CHECKING
 bool			irc::channel::is_user_op(user *obj)
 {
 	std::vector<irc::user>::iterator it;

 	for (it = _op.begin(); it != _op.end(); it++)
 	{
 		if (&(*it) == obj)
 			return true;
 	}
 	return false;
 }

 bool			irc::channel::is_user(user *obj)
 {
 	std::vector<irc::user>::iterator it;

 	for (it = _users.begin(); it != _users.end(); it++)
 	{
 		if (&(*it) == obj)
 			return true;
 	}
 	return false;
 }