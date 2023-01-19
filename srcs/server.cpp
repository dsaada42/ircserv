/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:11:15 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/19 13:08:07 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

// ----- Constructor / Destructor ------
irc::server::server(int port, std::string password) : 
    _port(port), _pass(password), _sock(AF_INET, SOCK_STREAM, TCP, port, INADDR_ANY, BACKLOG){
    //Add _sock to fd_set
    FD_ZERO(&_current_sockets);
    FD_SET(get_sock_fd(), &_current_sockets);   
}

irc::server::~server( void ){
    shutdown(_sock.get_sock(), 2);
    close(_sock.get_sock());
}

// ----- Getters -----
const std::string   & irc::server::get_pass( void ) const               { return(_pass); }
const int           & irc::server::get_port( void ) const               { return(_port); }
const int           & irc::server::get_sock_fd( void ) const            { return(_sock.get_sock()); }
fd_set              irc::server::get_current_sockets( void ) const      { return(_current_sockets); }

// ----- new FD_SET -----
 fd_set              *irc::server::get_fd_set( void )               {
    fd_set                          *new_set;
    std::vector<irc::user>::iterator it = _users.begin();
    
    new_set = new fd_set;
    FD_ZERO(new_set);
    FD_SET(get_sock_fd(), new_set);
    for (; it != _users.end(); it++){
        FD_SET((*it).get_fd(), new_set);
    }
    return (new_set);
}

// ----- Network -----
void        irc::server::clear_fd(const int & fd)       { FD_CLR(fd, &_current_sockets); }
void        irc::server::accept_connection( void )      {
    int newfd;

    newfd = accept(get_sock_fd(), (struct sockaddr *)NULL, NULL );
    if (newfd < 0){
        std::cerr << "Error accepting connection" << std::endl;
        exit(1);
    }
    //create user and store fd in class
    irc::user new_user;
    new_user.set_fd(newfd);
    _users.push_back(new_user);
    FD_SET(newfd, &_current_sockets);
}
void        irc::server::read_connection(const int & fd){
    std::string result;
    char        buffer[BUFF_SIZE];
    
    read(fd, buffer, sizeof(buffer) - 1); // blocking 
    result += buffer;
    bzero(&buffer, sizeof(buffer));
    std::cout << "[Message]\n" << result << "[End of Message]" << std::endl;
    //close(fd);
    result.clear();
}

void        irc::server::send_message(const int & fd, irc::message msg){
    std::string result;
    
    result = msg.get_message();
    write(fd, result.c_str(), result.size());
}

// ----- Debug / Print -----

void        irc::server::print_channel_users( irc::channel chan ){ (void)chan; }
void        irc::server::print_users( void ){
    std::vector<irc::user>::iterator it;
    int                              nb = 0;
    
    for (it = _users.begin(); it != _users.end(); it++){
        std::cout << "User No: " << nb << " has fd = " << (*it).get_fd() << std::endl;
        nb++;
    }
}
