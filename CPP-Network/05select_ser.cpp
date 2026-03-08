#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/select.h>
#define SER_PORT 8888
#define SER_IP "192.168.255.128"
using namespace std;

int main(){

    int sfd = socket(AF_INET,SOCK_STREAM,0);
    if(sfd==-1){
        perror("socket error");
        return -1;
    }

    struct sockaddr_in sin;
    sin.sin_family=AF_INET;
    sin.sin_port = htons(SER_PORT);
    sin.sin_addr.s_addr = inet_addr(SER_IP);


    if(bind(sfd,(sockaddr*)&sin,sizeof(sin))==-1){
        perror("bind error");
        return -1;
    }
    cout << "bind success" << endl;

    if(listen(sfd,128)==-1){
        perror("listen error");
        return -1;
    }
    cout << "listen success！" << endl;

    fd_set readfds,tempfds;
    FD_ZERO(&readfds);
    FD_SET(0,&readfds);
    FD_SET(sfd,&readfds);
    
    //设置最大描述符
    int maxfd = 3;
    //最新连接的客户端文件描述符
    int newfd;
    //存储最新连接的客户端地址
    struct sockaddr_in cin;
    socklen_t socklen = sizeof(cin);
    //持久存储连接的客户端地址
    struct sockaddr_in cin_arr[1024];

    while(1){
        tempfds = readfds;
        
        int res = select(maxfd+1,&tempfds,nullptr,nullptr,nullptr);
        if(res==-1){
            perror("select error");
            return -1;
        }
        else if(res==0){
            cout << "time out !"<<endl;
            return -1;
        }
        //执行到现在，集合中必定有触发的文件描述符,进行判断

        
        if(FD_ISSET(sfd,&tempfds)){
            newfd = accept(sfd,(sockaddr*)&cin,&socklen);
            if(newfd==-1){
                perror("accept error:");
                return -1;
            }
            printf("[%s:%d]: connect success!\n",inet_ntoa(cin.sin_addr),ntohs(cin.sin_port));
            
            FD_SET(newfd,&readfds);
            cin_arr[newfd] = cin;
            if(maxfd<newfd){
                maxfd = newfd;
            } 
        }

        
        for(int i=4;i<=maxfd;i++){
            if(FD_ISSET(i,&tempfds)){
                
                char buf[128];
                bzero(buf,sizeof(buf));
                int res = recv(i,buf,sizeof(buf),0);
                if(res==0){
                    cout << "对端已下线!" << endl;
                    //关闭文件描述符，更新maxfd
                    close(i);
                    FD_CLR(i,&readfds);
                    for(int k = maxfd;k>=0;k--){
                        if(FD_ISSET(k,&readfds)){
                            maxfd = k;
                            break;
                        }
                    }
                    continue;
                }
                printf("[%s:%d]:%s\n",inet_ntoa(cin_arr[i].sin_addr),ntohs(cin_arr[i].sin_port),buf);

                strcat(buf,"*v*");
                if(send(i,buf,sizeof(buf),0)==-1){
                    perror("send error");
                    return -1;
                }
                cout << "发送成功！！" << endl;
                
            }
        }

    }
    
    close(sfd);

    return 0;
}