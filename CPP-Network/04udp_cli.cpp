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
#define CLI_PORT 9999
#define CLI_IP "192.168.255.128"

int main(){
    int cfd = socket(AF_INET,SOCK_DGRAM,0);
    if(cfd==-1){
        perror("socket error");
        return -1;
    }

    sockaddr_in cin;
    cin.sin_family = AF_INET;
    cin.sin_port = htons(CLI_PORT);
    cin.sin_addr.s_addr = inet_addr(CLI_IP);

    if(bind(cfd,(sockaddr*)&cin,sizeof(cin))==-1){
        perror("bind error");
        return -1;
    }

    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(SER_PORT);
    sin.sin_addr.s_addr = inet_addr(SER_IP);
    
    char buf[128];
    while(1){
        bzero(buf,sizeof(buf));
        fgets(buf,sizeof(buf),stdin);
        buf[strlen(buf)-1]=0;
        if(sendto(cfd,buf,sizeof(buf),0,(sockaddr*)&sin,sizeof(sin))==-1){
            perror("sendto error");
            return -1;
        }
        if(recvfrom(cfd,buf,sizeof(buf),0,nullptr,nullptr)==-1){
            perror("recvfrom error");
            return -1;
        }

        std::cout << "服务器来电：" << buf << std::endl;

    }
    close(cfd);

    return 0;
}