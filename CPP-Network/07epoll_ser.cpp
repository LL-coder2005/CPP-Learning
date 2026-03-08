#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>

#define SER_PORT 8888
#define SER_IP "192.168.255.128"

using namespace std;

int main(){
    
    int sfd = socket(AF_INET,SOCK_STREAM,0);
    if(sfd==-1){
        perror("socket error");
        return -1;
    }

    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(SER_PORT);
    sin.sin_addr.s_addr = inet_addr(SER_IP);
    if(bind(sfd,(sockaddr*)&sin,sizeof(sin))==-1){
        perror("bind error");
        return -1;
    }

    if(listen(sfd,128)==-1){
        perror("listen error:");
        return -1;
    }

    int pfd = epoll_create(1);
    if(pfd==-1){
        perror("epoll error:");
        return -1;
    }
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = sfd;
    epoll_ctl(pfd,EPOLL_CTL_ADD,sfd,&ev);

    struct epoll_event pfds[128];
    int len = sizeof(pfds)/sizeof(pfds[0]);

    sockaddr_in cin;
    socklen_t socklen = sizeof(cin);

    char buf[128]="";

    while(1){
        int res = epoll_wait(pfd,pfds,len,-1);
        
        cout << "res = " << res << endl;
        for(int i = 0;i<res;i++){
            int fd =  pfds[i].data.fd;
            if(fd==sfd){
                int newfd = accept(fd,(sockaddr*)&cin,&socklen);
                if(newfd==-1){
                    perror("accept error:");
                    return -1;
                }
                epoll_event newev;
                newev.events = EPOLLIN;
                newev.data.fd = newfd;
                epoll_ctl(pfd,EPOLL_CTL_ADD,newfd,&newev);   
            }
            else{   
                bzero(buf,sizeof(buf));
                int res = recv(fd,buf,sizeof(buf),0);
                if(res==0){
                    cout << "对端已下线！" << endl;
                    close(fd);
                    epoll_ctl(pfd,EPOLL_CTL_DEL,fd,nullptr);
                    continue;
                }
                cout << buf << endl;
                strcat(buf,"******");
                if(send(fd,buf,sizeof(buf),0)==-1){
                    perror("send error");
                    return -1;
                }
            }
        }
    }
    
    close(pfd);
    close(sfd);

    return 0;

}