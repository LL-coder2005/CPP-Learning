#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#define SER_PORT 8888
#define SER_IP "192.168.255.128"

using namespace std;
int main(int argc,const char* argv[]){
    int sfd = socket(AF_INET,SOCK_STREAM,0);
    if(sfd==-1){
        perror("socket error:");
        return -1;
    }

    cout<<"socket sfd: " <<sfd<<endl;

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(SER_PORT);
    sin.sin_addr.s_addr = inet_addr(SER_IP);

    if(bind(sfd,(sockaddr*)&sin,sizeof(sin))==-1){
        perror("bind error:");
        return -1;
    }

    if(listen(sfd,128)==-1){
        perror("listen error:");
        return -1;
    }

    sockaddr_in cin;
    socklen_t len = sizeof(cin);
    int newfd = accept(sfd,(sockaddr*)&cin,&len);
    if(newfd==-1){
        perror("accept error:");
        return -1;
    }
    char buf[128];
    while(1){
        bzero(buf,sizeof(buf));
        int res = recv(newfd,buf,sizeof(buf),0);
        if(res==0){
            cout << "对端已下线" << endl;
            break;
        }
        cout << buf << endl;
        strcat(buf,"*v*");
        if(send(newfd,buf,sizeof(buf),0)==-1){
            perror("send error:");
            return -1;
        }
        cout << "发送成功" << endl;
    }   

    close(newfd);
    close(sfd);

    cout << "hello world" << endl;

    return 0;
    
}