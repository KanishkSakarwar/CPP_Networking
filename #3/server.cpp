#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring> 

#define PORT "8080"

int main(){
    struct addrinfo hints,*res;
    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    struct sockaddr_storage client_addr;
    socklen_t addr_size;
    int status = getaddrinfo(NULL,PORT,&hints,&res);
    if(status<0){
        std::cout<<"error\n";
        return 1;
    }
    int sock_fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    if(sock_fd<0){
        std::cout<<"socket error\n";
        return 1;
    }
    if(bind(sock_fd,res->ai_addr,res->ai_addrlen)<0){
        std::cout<<"bind error\n";
        return 1;
    }
    addr_size = sizeof(client_addr);
    char buffer[1024];
    ssize_t bytes_received = recvfrom(sock_fd,buffer,sizeof(buffer)-1,0,(struct sockaddr*)&client_addr,&addr_size);
    if(bytes_received<0){
        close(sock_fd);
        free(res);
        std::cout<<"recvfrom error\n";
        return 1;
    }   
    buffer[bytes_received] = '\0';
    std::cout<<"Received char: "<<buffer<<"\n";
    buffer[0] = (buffer[0]=='z')?'a':buffer[0]+1;

    struct sockaddr_storage client2;
    socklen_t client2len = sizeof(client2);
    std::cout<<"Waiting for client 2 to connect...\n";
    char buffer2[1024];
    ssize_t ready_or_not = recvfrom(sock_fd,buffer2,sizeof(buffer2)-1,0,(struct sockaddr*)&client2,&client2len);
    if(ready_or_not<0){
        close(sock_fd);
        free(res);
        std::cout<<"recvfrom error\n";
        return 1;
    }
    std::cout<<"Connection established with client 2\n";

    ssize_t bytes_sent = sendto(sock_fd,buffer,1,0,(struct sockaddr*)&client2,client2len);
    if(bytes_sent<0){
        close(sock_fd);
        freeaddrinfo(res);
        std::cout<<"sendto error\n";
        return 1;
    }
    std::cout<<"successfully sent to client 2\n";
    close(sock_fd);
    freeaddrinfo(res);
    return 0;
}