/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 11:08:52 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/26 09:40:38 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"

int ft_admin( irc::message * msg ){
    (void)msg;
    return (SUCCESS);    
}
int ft_cap( irc::message * msg ){
    (void)msg;
    return (SUCCESS);    
}
int ft_error( irc::message * msg ){
    (void)msg;
    return (SUCCESS);    
}
int ft_info( irc::message * msg ){
    (void)msg;
    return (SUCCESS);    
}
int ft_invite( irc::message * msg ){
    (void)msg;
    return (SUCCESS);    
}
int ft_join( irc::message * msg ){
    (void)msg;
    return (SUCCESS);    
}
int ft_kick( irc::message * msg ){
    (void)msg;
    return (SUCCESS);    
}
int ft_kill( irc::message * msg ){
    (void)msg;
    return (SUCCESS);    
}
int ft_list( irc::message * msg ){
    (void)msg;
    return (SUCCESS);    
}
int ft_mode(irc::message * msg){
    (void)msg;
    return (SUCCESS);    
}
int ft_names(irc::message * msg){
    (void)msg;
    return (SUCCESS);    
}
int ft_nick(irc::message * msg){
    (void)msg;
    return (SUCCESS);    
}
int ft_notice(irc::message * msg){
    (void)msg;
    return (SUCCESS);    
}
int ft_oper(irc::message * msg){
    (void)msg;
    return (SUCCESS);    
}
int ft_pass(irc::message * msg){
    (void)msg;
    return (SUCCESS);    
}
int ft_part(irc::message * msg){
    (void)msg;
    return (SUCCESS);    
}
int ft_ping(irc::message * msg){//n'arrivera qu'en cas de reponse d'un autre serveur : inutile 
    (void)msg;
    //possible numeric errors :  ERR_NOORIGIN / ERR_NOSUCHSERVER
    return (SUCCESS);    
}
int ft_pong(irc::message * msg){
    if (msg->get_params().empty()){
        std::cout << "PONG: no origin specified" << std::endl;   
        //reply no origin giver ERR_NOORIGIN
    }
    else if (msg->get_params() != SERVER_NAME){
        std::cout << "PONG: no such server " << msg->get_params() << std::endl;
        //reply no such server ERR_NOSUCHSERVER
    }
    else{
        std::cout << "Wouhou !" << std::endl;
        //do stuff
    }
    return (SUCCESS);
}
int ft_privmsg(irc::message * msg){
    (void)msg;
    return (SUCCESS);    
}
int ft_quit(irc::message * msg){
    (void)msg;
    return (SUCCESS);    
}
int ft_stats(irc::message * msg){
    (void)msg;
    return (SUCCESS);    
}
int ft_time(irc::message * msg){
    (void)msg;
    return (SUCCESS);    
}
int ft_topic(irc::message * msg){
    (void)msg;
    return (SUCCESS);    
}
int ft_user(irc::message * msg){
    (void)msg;
    return (SUCCESS);    
}
int ft_version(irc::message * msg){
    (void)msg;
    return (SUCCESS);    
}
int ft_who(irc::message * msg){
    (void)msg;
    return (SUCCESS);    
}
int ft_whois(irc::message * msg){
    (void)msg;
    return (SUCCESS);    
}
int ft_whowas(irc::message * msg){
    (void)msg;
    return (SUCCESS);
}