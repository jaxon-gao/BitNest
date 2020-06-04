#ifndef NEST_NET_TOOLS
#define NEST_NET_TOOLS

#include <msg/msg.h>
#include <string>
#include <FileSystem/StorageData.h>
using namespace std;
//设置套接字非阻塞
int setnonblocking(int fd);

//设置端口重用
void setreuseaddr(int sock);

//将含有header的buffer接收到对应数据结构中
uint8_t *header_buffer(const msg_header &h);

//将header转化为字节流
msg_header *buffer_header(uint8_t *buffer);

//发送消息包括消息头和消息体，到套接字文件描述符
void send_msg(int sockfd, const msg_header &h, const string &m);

//从文件描述符接收消息头
char recv_header(int sockfd, msg_header &h);

//从套接字接收字符串（stirng）
void recv_content(int sockfd, string &m);

//从文件描述符接收存储
StorageData *recv_file(int fd);

//将文件存储发送至套接字
bool send_file(int fd, const StorageData &d);

//将文件块发送至套接字
bool send_fblock(int fd, FileBlock *d);

//从文件描述符接收文件块
FileBlock *recv_fblock(int fd);

#endif