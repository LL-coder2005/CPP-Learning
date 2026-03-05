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

using namespace std;

int main(int argc,const char* argv[]){

    int sfd = socket(AF_INET,SOCK_STREAM,0);
    if(sfd==-1){
        perror("socket error:");
        return -1;
    }

    struct sockaddr_in cin;
    cin.sin_family = AF_INET;
    cin.sin_port = htons(CLI_PORT);
    cin.sin_addr.s_addr = inet_addr(CLI_IP);

    if(bind(sfd,(sockaddr*)&cin,sizeof(cin))==-1){
        perror("bind error:");
        return -1;
    }

    sockaddr_in sin;
    sin.sin_family=AF_INET;
    sin.sin_port = htons(SER_PORT);
    sin.sin_addr.s_addr = inet_addr(SER_IP);
    
    if(connect(sfd,(sockaddr*)&sin,sizeof(sin))==-1){
        perror("connect error");
        return -1;
    }

    char buf[128];
    while(1){
        bzero(buf,sizeof(buf));
        fgets(buf,sizeof(buf),stdin);
        buf[strlen(buf)-1]=0;
        if(send(sfd,buf,sizeof(buf),0)==-1){
            perror("send error:");
            return -1;
        }
        recv(sfd,buf,sizeof(buf),0);
        cout << buf << endl;
    }
    close(sfd);

    return 0;
}