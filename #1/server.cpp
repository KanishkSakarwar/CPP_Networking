#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>

#define PORT 8080

int main(){
    int server_fd, client_fd;
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int status = getaddrinfo(NULL, "8080", &hints, &res);
    if(status != 0){
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        return 1;
    }

    server_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(server_fd == -1){
        std::cerr << "Socket creation error" << std::endl;
        freeaddrinfo(res);
        return 1;
    }

    bind(server_fd,res->ai_addr,res->ai_addrlen);
}