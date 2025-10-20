#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>

#define PORT "8080"

int main(){
    int server_fd, client_fd;
    struct addrinfo hints, *res;
    struct sockaddr_storage client_addr;
    socklen_t addr_size;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int status = getaddrinfo(NULL, PORT, &hints, &res);
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

    listen(server_fd,10);

    std::cout << "Server listening on port " << PORT << std::endl;
    
    addr_size = sizeof(client_addr);
    
    client_fd = accept(server_fd,(struct sockaddr *)&client_addr, &addr_size);
    if(client_fd == -1){
        std::cerr << "Accept error" << std::endl;
        close(server_fd);
        freeaddrinfo(res);
        return 1;
    }
    char buffer[1024];
    int bytes_received = recv(client_fd, buffer, sizeof(buffer)-1, 0);
    if(bytes_received == -1){
        std::cerr << "Receive error" << std::endl;
        close(client_fd);
        close(server_fd);
        freeaddrinfo(res);
        return 1;
    }
    buffer[bytes_received] = '\0';
    std::cout << "Message from client: " << buffer << std::endl;
    close(client_fd);
    close(server_fd);
    freeaddrinfo(res);
    return 0;
}