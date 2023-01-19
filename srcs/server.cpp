/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:11:15 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/19 17:17:16 by dsaada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

// ----- Constructor / Destructor ------
irc::server::server(int port, std::string password) : 
    _port(port), _pass(password), _sock(AF_INET, SOCK_STREAM, TCP, port, INADDR_ANY, BACKLOG){
}

irc::server::~server( void ){
    shutdown(_sock.get_sock(), 2);
    close(_sock.get_sock());
}

// ----- Getters -----
const std::string   & irc::server::get_pass( void ) const       { return(_pass); }
const int           & irc::server::get_port( void ) const       { return(_port); }
const int           & irc::server::get_sock_fd( void ) const    { return(_sock.get_sock()); }

// ----- new FD_SET -----
 fd_set              *irc::server::get_fd_set( void )           {
    fd_set                           *new_set;
    std::map<int, irc::user*>::iterator it = _users.begin();
    
    new_set = new fd_set;
    FD_ZERO(new_set);
    FD_SET(get_sock_fd(), new_set);
    for (; it != _users.end(); it++){
        FD_SET((*it).first, new_set);
    }
    return (new_set);
}

// ----- Network -----
void        irc::server::accept_connection( void )              {
    int newfd;

    newfd = accept(get_sock_fd(), (struct sockaddr *)NULL, NULL );
    if (newfd < 0){
        std::cerr << "Error accepting connection" << std::endl;
        exit(1);
    }
    // create new user (default)
    irc::user * new_user = new irc::user;
    // set new user fd
    new_user->set_fd(newfd);
    // add pair<fd, user> to map of users 
    _users.insert(std::make_pair(newfd, new_user));
    //faire en sorte de verifier la bonne reception des 3 trames PASS NICK USER puis update le user
    //...
    // sending notice meanwhile
    irc::message test("\r\nNOTICE * Checking identity...\r\n");
    send_message(newfd, test);
    //after verification
    irc::message reply("\r\n:my_server 001 dsaada :Welcome to our shitty IRC server\r\n");
    send_message(newfd, reply);
    //les envois de message devront se faire avec FD_ISSET(fd, write_sockets) , pour le moment on le fait a l'arrache et le client y croit
}
void        irc::server::read_connection(const int & fd)        {
    std::string result;
    char        buffer[BUFF_SIZE];
    
    read(fd, buffer, sizeof(buffer) - 1);
    result = buffer;
    bzero(&buffer, sizeof(buffer));
    std::cout << "-> " << result << std::endl;
}

void        irc::server::send_message(const int & fd, irc::message msg){
    std::string result;
    
    result = msg.get_message();
    write(fd, result.c_str(), result.size());
}

// ----- Run main loop -----
int         irc::server::run( void ){

    while( true ){
        read_sockets = get_fd_set();
        write_sockets = get_fd_set();

        if (select(FD_SETSIZE, read_sockets, write_sockets, NULL, NULL) < 0 ) {
            perror("select returned an error");
            exit(1);
        }
        for (int i = 0; i < FD_SETSIZE; i++){
            if (FD_ISSET(i, read_sockets)){
                if (i == get_sock_fd()){
                    std::cout << "New connection incoming" << std::endl;
                    accept_connection();
                }
                else {
                    std::cout << "Received message from established connection" << std::endl;
                    read_connection(i);
                }
            }
            else if (FD_ISSET(i, write_sockets)){
                // std::cout << "Sending message to established connection " << i << std::endl;
                // //irc::message test("\r\nNOTICE * Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur sit amet varius tortor, at molestie turpis. Phasellus eget quam ac erat venenatis ornare. Vestibulum a metus ac nulla lacinia sollicitudin eget at risus. Morbi volutpat porttitor turpis, ac faucibus lacus faucibus quis. Morbi nec elementum massa. Quisque malesuada aliquet dui, sit amet auctor dui lacinia nec. In hac habitasse platea dictumst. Quisque egestas dictum ante vel pellentesque. Nulla consequat malesuada tortor, sed posuere diam ornare ut. Donec pellentesque tortor nulla, id aliquet orci iaculis ac. Morbi tempus justo at eros ornare, in posuere leo consectetur. Sed eget eros sed neque tristique egestas sagittis a erat. Aenean eget mauris vel purus ultricies pulvinar sed sit amet tellus. In at blandit dui. Mauris lacinia ipsum eget mauris dictum, dictum congue nunc blandit. Sed scelerisque vulputate eros, in vulputate augue porttitor non. Praesent laoreet quam vel eleifend consequat. Phasellus vitae fermentum ac.\r\n");
                // //send_message(i, test);
            }
        }
        delete read_sockets;
        delete write_sockets;
    }
}

// ----- Debug / Print -----

void        irc::server::print_channel_users( irc::channel chan ){ (void)chan; }
void        irc::server::print_users( void ){
    std::map<int, irc::user*>::iterator it;
    int                              nb = 0;
    
    for (it = _users.begin(); it != _users.end(); it++){
        std::cout << "User No: " << nb << " has fd = " << (*it).first << std::endl;
        nb++;
    }
}
