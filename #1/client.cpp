#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>

#define PORT "8080"

int main(){
    int client_fd,server_fd;
    char msg[1024];
    int len, bytes_sent;
    struct addrinfo *res, hints;
    memset(&hints, 0, sizeof(hints));
    struct sockaddr_storage server_addr;
    socklen_t addrlen;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    int status = getaddrinfo("127.0.0.1", PORT, &hints, &res);
    if(status != 0){
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        return 1;
    }
    client_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(client_fd == -1){
        std::cerr << "Socket creation error" << std::endl;
        freeaddrinfo(res);
        return 1;
    }
    int flag = connect(client_fd, res->ai_addr, res->ai_addrlen);
    if(flag == -1){
        std::cerr << "Connection error" << std::endl;
        close(client_fd);
        freeaddrinfo(res);
        return 1;
    }
    std::cout << "Connected to server" << std::endl;
    while(true){
        std::cin >> msg;
        len = strlen(msg);
        bytes_sent = send(client_fd, msg, len, 0);
        if(bytes_sent == -1){
            std::cerr << "Send error" << std::endl;
            close(client_fd);
            freeaddrinfo(res);
            return 1;
        }
        if(bytes_sent == 0) break; // Connection closed
        std::cout << "Message sent to server: " << msg << std::endl;
    }
    close(client_fd);
    freeaddrinfo(res);
    return 0;
}