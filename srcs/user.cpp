/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 10:54:59 by dsaada            #+#    #+#             */
/*   Updated: 2023/02/07 17:04:29 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "user.hpp"

// ----- Constructor default -----
irc::user::user( int fd , unsigned long timestamp )
    : _fd(fd), _connected(false), _ping(false), _remain(0), _timestamp(timestamp), _pass(false){ 
    bzero(_buff, BUFF_SIZE);
    _creation_time = timestamp;
    _sent = 0;
    _sentbytes = 0;
    _received = 0;
    _receivedbytes = 0;
}

// ----- Copy Constructor -----
irc::user::user( const irc::user & x)
    : _username(x.username()), _nickname(x.nickname()), _fullname(x.fullname()), _fd(x.fd()), _remain(0), _timestamp(x.timestamp()){}
// ----- Destructor -----
irc::user::~user( void ){
    shutdown(_fd, 2);
    close(_fd);
}

// ----- Operator= -----
irc::user & irc::user::operator= (const irc::user & x){
    (void)x;
    return (*this);
}

// ----- Getters -----
const str   & irc::user::username   ( void ) const  { return(_username);}
const str   & irc::user::fullname   ( void ) const  { return(_fullname);}
const str   & irc::user::nickname   ( void ) const  { return(_nickname);}
const int   & irc::user::fd         ( void ) const  { return(_fd);}
const unsigned long & irc::user::timestamp( void ) const { return(_timestamp);}
const bool  & irc::user::connected  ( void ) const  { return(_connected);}
const bool  & irc::user::ping       ( void ) const  { return(_ping);}
const bool  & irc::user::pass       ( void ) const  { return(_pass);}
const str   & irc::user::mode       ( void ) const  { return(_mode);}
const str   & irc::user::oldnick    ( void ) const  { return(_oldnick);}
const unsigned long & irc::user::creation_time ( void ) const { return(_creation_time);}
// ----- Setters -----
void irc::user::set_username(const str & username)  { _username = username; }
void irc::user::set_fullname(const str & fullname)  { _fullname = fullname; }
void irc::user::set_nickname(const str & nickname)  { 
    _oldnick = _nickname;
    _nickname = nickname;
}
void irc::user::set_connected(const bool & connected){ _connected = connected; }
void irc::user::set_ping(const bool & ping)         { _ping = ping; }
void irc::user::set_pass(const bool & pass)         { _pass = pass; }

// ----- MODE ------
bool    irc::user::is_mode( char c ){
    
    if (_mode.find(c) != str::npos)
        return (true);
    return (false);
}
void    irc::user::change_mode(char c, bool plus){
    str::size_type  pos;

    if (plus){
        if (_mode.find(c) == str::npos){
            _mode += c;
        }
    }
    else{
        if ((pos = _mode.find(c)) != str::npos){
           _mode.erase(pos, 1);
        }
    }
        
}

// ----- Read Connexion -----
int irc::user::read_connection(void)                {
    int cpt;
     
    if ((cpt = read(_fd, _buff + _remain, sizeof(_buff) - _remain - 1)) <= 0){
        return (FAILURE);
    }
    _receivedbytes += (unsigned long)cpt;
    return (SUCCESS);
}

// ----- Extract one message -----
irc::message *irc::user::extract_message(str delim){
    irc::message    *newmsg;
    str             buff = _buff;
    str::size_type  i;

    if ((i = buff.find(delim)) != str::npos){
        newmsg = new message(buff.substr(0, i), _fd);
        buff = buff.substr(i + delim.size(), buff.size());
        bzero(_buff, BUFF_SIZE);
        _remain = buff.size();
        for (i = 0; i < _remain; i++)
            _buff[i] = buff[i];
        if (_connected)
            new_timestamp();
        _received++;
        return (newmsg);
    }
    else{
        _remain = buff.size();
        return (NULL);
    }
}

// ----- Stats -----
void irc::user::incr_sent_cmd(void){
    _sent++;
}
void irc::user::incr_sent_bytes(int n){
    _sentbytes += (unsigned long)n;
}
str irc::user::connection_stats( void ){
    unsigned long opentime;
    struct timeval  time_now;
    gettimeofday(&time_now, NULL);
    opentime = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
    opentime -= _creation_time;
    
    return (_nickname + " " + "[0 msg in queue] [" + SSTR(_sent) + " msg sent] [" + SSTR(_sentbytes) + " bytes sent] ["
        + SSTR(_received) + " msg received] [" + SSTR(_receivedbytes) + " bytes received] "
        + ft_ms_to_duration(opentime) + " elapsed since creation");    
}

// ----- New Timestamp -----
void irc::user::new_timestamp(void){
    struct timeval  time_now;
    gettimeofday(&time_now, NULL);
    _timestamp = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
}
// ----- Print User -----
void irc::user::print(void){
    std::cout << "============> User <=============" << std::endl;
    std::cout << "-> Username       : " << _username << std::endl;
    std::cout << "-> Fullname       : " << _fullname << std::endl;
    std::cout << "-> Nickname       : " << _nickname << std::endl;
    std::cout << "-> Modes          : " << _mode << std::endl;
    std::cout << "-> Fd             : " << _fd << std::endl;
    std::cout << "-> Connected      : " << _connected << std::endl;
    std::cout << "-> Ping           : " << _ping << std::endl;
    std::cout << "-> Pass           : " << _pass << std::endl;
    std::cout << "-> Received cmds  : " << _received << std::endl;
    std::cout << "-> Received bytes : " << _receivedbytes << std::endl;
    std::cout << "-> Sent cmds      : " << _sent << std::endl;
    std::cout << "-> Sent bytes     : " << _sentbytes << std::endl;
}