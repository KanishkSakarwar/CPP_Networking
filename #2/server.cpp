#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>

#define PORT "8080"

int main(){
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    struct sockaddr_storage client_addr;
    socklen_t addr_size;
    int status = getaddrinfo(NULL, PORT, &hints, &res);
    if(status<0){
        std::cout<<"getaddrinfo error"<<std::endl;
        freeaddrinfo(res);
        return 1;
    }
    int server_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(server_fd<0){
        std::cout<<"socket error"<<std::endl;
        freeaddrinfo(res);
        return 1;
    }
    bind(server_fd, res->ai_addr, res->ai_addrlen);

    std::cout << "Server listening on port " << PORT << std::endl;

    addr_size = sizeof(client_addr);

    char buffr[1024];
    
    int bytes_received = recvfrom(server_fd, buffr, sizeof(buffr)-1, 0, (struct sockaddr *)&client_addr, &addr_size);

    buffr[bytes_received] = '\0';

    std::cout << "Message from client: " << buffr << std::endl;

    close (server_fd);
    freeaddrinfo(res);
    return 0;
}