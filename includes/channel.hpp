/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 09:20:13 by dsaada            #+#    #+#             */
/*   Updated: 2023/02/04 00:42:18 by mlaouedj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include "ircserv.hpp"
# include "user.hpp"

namespace irc{
    
	class channel {
	    
		 private:
		    std::vector<irc::user*>	_users;
		    std::vector<irc::user*>	_op;
		    std::vector<irc::user*>	_invit;
		    std::vector<irc::user*>	_ban;
		    str				_name;
		    str				_password;
		    str				_topic;
		    str				_modes;
		    int				_nb_max_users;

		 public:
		    //CONSTRUCTOR & DESTRCUTOR
		    channel(str name, str pass, str topic, str modes);
		    ~channel(void);
		    //GETTERS
		    std::vector<irc::user*>	get_users(void) const;
		    std::vector<irc::user*>	get_op(void) const;
		    std::vector<irc::user*>	get_invit(void) const;
		    std::vector<irc::user*>	get_ban(void) const;
		    const str				&get_name(void) const;
		    const str				&get_password(void) const;
		    const str				&get_topic(void) const;
		    const str				&get_modes(void) const;
		    const int				&get_nb_max(void) const;
			//SETTERS
			void			set_topic(const str &topic);
		    //CHECKING
			bool			is_empty(void);
		    bool			is_op(user *obj);
		    bool			is_user(user *obj);
		    bool			is_invit(user *obj);
		    bool			is_ban(user *obj);
		    bool			is_mode(str m);
		    //ADDING
		    void			add_user(user *obj);
		    void			add_op(user *obj);
		    void			add_invit(user *obj);
		    void			add_ban(user *obj);
		    void			add_mode(str m);
		    void			add_nb_max(int nb);
		    //REMOVING
		    void			remove_user(user *obj);
		    void			remove_op(user *obj);
		    void			remove_invit(user *obj);
		    void			remove_ban(user *obj);
			//HANDLING USER DELETIOn/QUIT/KILL
			void			delete_user(user *obj);
		    //UTILS
		    bool			is_num(str s);
			
    
    };

}

#endif
