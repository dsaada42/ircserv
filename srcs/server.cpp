/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsaada <dsaada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:11:15 by dsaada            #+#    #+#             */
/*   Updated: 2023/01/20 11:25:29 by dsaada           ###   ########.fr       */
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
const int           & irc::server::port( void ) const           { return(_port); }
const int           & irc::server::sock_fd( void ) const        { return(_sock.get_sock()); }

// ----- Network -----
void        irc::server::accept_connection( void )              {
    int newfd;

    newfd = accept(sock_fd(), (struct sockaddr *)NULL, NULL );
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

// ----- Exception handler -----
void        irc::server::handle_except_set(void){
    // cette fonction devra gerer les exceptions recues par select (disconnect? error?)
    // man select pour savoir 
}

// ----- Read Handler -----
void        irc::server::handle_read_set( void ){
    // cette fonction a pour buts:
    //    - de gerer la connexion de nouveaux users
    //    - de lire tous les messages recus de chaque user 
    // elle appelera la fonction de parsing, interpretation des messages 
    // et entrainera la generation des messages dans un container queue (FIFO)  
    for (int i = 0; i < FD_SETSIZE; i++){
            if (FD_ISSET(i, &read_sockets)){
                if (i == sock_fd()){
                    std::cout << "New connection incoming" << std::endl;
                    accept_connection();
                }
                else {
                    std::cout << "Received message from established connection" << std::endl;
                    read_connection(i);
                }
            }
        }
}

// ----- Write Handler -----
void        irc::server::handle_write_set(void){
    // cette fonction devra gerer l'envoi de la queue de messages 
    // mettre en attente les messages ne poouvant pas etre envoyes
    // calculer les problemes de trop longue attente pour l'envoi d'un message ? 
    // clean les messages dedies a des users deconnectes ?
}

// ----- Main loop -----
int         irc::server::run( void ){

    while( true ){
        update_sets();
        
        if (select(FD_SETSIZE, &read_sockets, &write_sockets, &except_sockets, NULL) < 0 ) {
            perror("select returned an error");
            exit(1);
        }
        handle_except_set(); // mysterieux
        handle_read_set();
        handle_write_set();
    }
}

// ----- Select helper -----
 void              irc::server::update_sets( void )           {
    // genere les sets a passer en parametre a select a chaque debut de la main loop
    std::map<int, irc::user*>::iterator it = _users.begin();
    
    FD_ZERO(&read_sockets);
    FD_ZERO(&write_sockets);
    FD_ZERO(&except_sockets);
    FD_SET(sock_fd(), &read_sockets);
    FD_SET(sock_fd(), &write_sockets);
    FD_SET(sock_fd(), &except_sockets);
    for (; it != _users.end(); it++){
        FD_SET((*it).first, &read_sockets);
        FD_SET((*it).first, &write_sockets);
        FD_SET((*it).first, &except_sockets);
    }
}

// ----- Debug / Print -----

void        irc::server::print_users( void ){
    std::map<int, irc::user*>::iterator it;
    int                              nb = 0;
    
    for (it = _users.begin(); it != _users.end(); it++){
        std::cout << "User No: " << nb << " has fd = " << (*it).first << std::endl;
        nb++;
    }
}
