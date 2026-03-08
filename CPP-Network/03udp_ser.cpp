#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#define SER_PORT 8888
#define SER_IP "192.168.255.128"

int main(){
    int sfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sfd==-1){
        perror("socket error");
        return -1;
    }

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(SER_PORT);
    sin.sin_addr.s_addr = inet_addr(SER_IP);
    if(bind(sfd,(sockaddr*)&sin,sizeof(sin))==-1){
        perror("bind error");
        return -1;
    }

    sockaddr_in cin;
    socklen_t socklen = sizeof(cin);
    char buf[128];
    while(1){
        bzero(buf,sizeof(buf));
        if(recvfrom(sfd,buf,sizeof(buf),0,(sockaddr*)&cin,&socklen)==0){
            std::cout << "对端已下线" <<std::endl;
            break;
        }
        strcat(buf,"*v*");
        if(sendto(sfd,buf,sizeof(buf),0,(sockaddr*)&cin,socklen)==-1){
            perror("sendto error");
            return -1;
        }
    }
    close(sfd);
    return 0;
}

