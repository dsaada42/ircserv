#include <iostream>
#include <string>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#define SERVER_PORT 6667
#define BUFF_SIZE 1024
#define SA struct sockaddr

int main(void){

    int                 listenfd;
    int                 connfd;
    struct sockaddr_in  servaddr;
    std::string         result;
    char                buffer[BUFF_SIZE];

    //-----  create socket -----
    //AF_INET = Address Family - Internet
    //SOCK_STREAM = Stream Socket
    //0 = use TCP
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        std::cerr << "Can't create socket" << std::endl;
        return (-1);
    }
    //----- create and setup address -----
    //htons = host to network, short
    //htonl = host to network, long 
    //both these functions allow little endian and big endian computers to talk to each other on the same standard
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family         = AF_INET;
    servaddr.sin_addr.s_addr    = htonl(INADDR_ANY);
    servaddr.sin_port           = htons(SERVER_PORT);

    //----- bind -----
    if ((bind(listenfd, (SA *) &servaddr, sizeof(servaddr))) < 0)
    {
        std::cerr << "Can't bind to IP/port" << std::endl;
        return (-2);
    }
    //----- listen -----
    if ((listen(listenfd, 10)) < 0){
        std::cerr << "Can't listen" << std::endl;
    }
    //----- accept a call -----
    while (1){
        int n = 0;
        // struct sockaddr_in  addr;
        // socklen_t          addr_len;

        std::cout << "Waiting for a connection on port " << SERVER_PORT << std::endl;
        //Accept will block until incoming connexion 
        connfd = accept(listenfd, (SA *) NULL, NULL);

        std::cout << "received message" << std::endl;
        while ((n = read(connfd, buffer, sizeof(buffer) - 1)) > 0){
            result += buffer;
        }
        std::cout << "[Message] " << result << " [End of Message]" << std::endl;
        result.clear();
    }


}