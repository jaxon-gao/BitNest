#include<tools/test_net_tool.h>
#include<iostream>
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <string>
using namespace std;
#include <fcntl.h>
#ifdef _WIN32
#include <Winsock2.h>
#include <Ws2tcpip.h>
#define strncasecmp _strnicmp
#else
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#endif
using namespace std;
msg_header headers[2];
void *sub_thread(void* args){
    int cli_fd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in ser_addr;
    
    ser_addr.sin_family=AF_INET;
    ser_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    ser_addr.sin_port=htons(9999);

    connect(cli_fd,(sockaddr*)&ser_addr,sizeof(sockaddr));
    string msg = "hello world";
    send_msg(cli_fd,headers[0],msg);
}

bool test_net_tool(){
    string msg;
    msg_header h;
    h.msg_size=1;
    h.pk[0]=1024;
    h.pk[1]=2048;
    h.data[0]=4096;
    h.data[1]=11111;
    h.kind='c';
    uint8_t * p1=header_buffer(h);
    msg_header *hh=buffer_header(p1);
    if(h.data[0]!=hh->data[0]){
        return false;
    }
    if(h.data[1]!=hh->data[1]){
        return false;
    }
    if(h.pk[1]!=hh->pk[1]){
        return false;
    }
    if(h.pk[0]!=hh->pk[0]){
        return false;
    }
    if(h.kind!=hh->kind){
        return false;
    }
    if(h.msg_size!=hh->msg_size){
        return false;
    }
    headers[0]=*hh;

    //test: open server
    int serv_fd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr_ser;
    
    addr_ser.sin_family=AF_INET;
    addr_ser.sin_addr.s_addr=htonl(INADDR_ANY);
    addr_ser.sin_port=htons(9999);

    socklen_t len_ser = sizeof(addr_ser);
    bind(serv_fd,(struct sockaddr*)&addr_ser,len_ser);

    listen(serv_fd,1);

    sockaddr_in addrClient;
    int len=sizeof(sockaddr);

    pthread_t thread;
    int ret = pthread_create(&thread,NULL,sub_thread,NULL);

    int cli_fd = accept(serv_fd,(sockaddr*)&addrClient,(socklen_t*)&len);

    recv_header(cli_fd,headers[1]);    
    recv_content(cli_fd,msg); 
    if(headers[0].data[0]!=headers[1].data[0]){
        return false;
    }
    if(headers[0].data[1]!=headers[1].data[1]){
        return false;
    }
    if(headers[0].pk[1]!=headers[1].pk[1]){
        return false;
    }
    if(headers[0].pk[0]!=headers[1].pk[0]){
        return false;
    }
    if(headers[0].kind!=headers[1].kind){
        return false;
    }
    if(headers[0].msg_size!=headers[1].msg_size){
        return false;
    }
    if(msg!="hello world"){
        return false;
    }

    cout<<"test: net tool test finished"<<endl;
    return true;
}