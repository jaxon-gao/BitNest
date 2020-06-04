#ifndef NEST_NET_TOOLS
#define NEST_NET_TOOLS
#include <tools/net_tool.h>
#include <msg/msg.h>
#include <stdio.h>
#include <uint256.h>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>   /* socket类定义需要*/
#include <sys/epoll.h>    /* epoll头文件 */
#include <fcntl.h>        /* nonblocking需要 */
#include <sys/resource.h> /* 设置最大的连接数需要setrlimit */
#include <iostream>
#include <iomanip>
using namespace std;
int setnonblocking(int fd)
{
    if (fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0) | O_NONBLOCK) == -1)
    {
        printf("Set blocking error : %d\n", errno);
        return -1;
    }
    return 0;
}
void setreuseaddr(int sock)
{
    int opt;
    opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(&opt)) < 0)
    {
        perror("setsockopt");
        exit(1);
    }
}

uint8_t *header_buffer(const msg_header &h)
{
    uint8_t *buff = new uint8_t[134];

    buff[0] = (uint8_t)h.kind;
    vector<uint8_t> bits = h.pk[0].export_bits();
    int index = 1;
    for (int i = 0; i < bits.size(); i++)
    {
        buff[index++] = bits[i];
    }
    bits = h.pk[1].export_bits();
    for (int i = 0; i < bits.size(); i++)
    {
        buff[index++] = bits[i];
    }
    bits = h.data[0].export_bits();
    for (int i = 0; i < bits.size(); i++)
    {
        buff[index++] = bits[i];
    }
    bits = h.data[1].export_bits();
    for (int i = 0; i < bits.size(); i++)
    {
        buff[index++] = bits[i];
    }
    buff[index++] = (uint8_t)(((h.msg_size) & 0xff000000) >> 24);
    buff[index++] = (uint8_t)(((h.msg_size) & 0x00ff0000) >> 16);
    buff[index++] = (uint8_t)(((h.msg_size) & 0x0000ff00) >> 8);
    buff[index++] = (uint8_t)(((h.msg_size) & 0x000000ff));
    return buff;
}
msg_header *buffer_header(uint8_t *buffer)
{
    msg_header *h = new msg_header();
    string ans;
    int index = 0;
    h->kind = buffer[index++];
    stringstream ss;
    for (int i = 0; i < 32; i++)
    {
        ss << setw(2) << setfill('0') << hex << (int)buffer[index++];
    }
    ss >> ans;
    ss.clear();
    h->pk[0] = uint256(ans);
    ans.clear();
    for (int i = 0; i < 32; i++)
    {
        ss << setw(2) << setfill('0') << hex << (int)buffer[index++];
    }
    ss >> ans;
    ss.clear();

    h->pk[1] = uint256(ans);
    ans.clear();

    for (int i = 0; i < 32; i++)
    {
        ss << setw(2) << setfill('0') << hex << (int)buffer[index++];
    }
    ss >> ans;
    ss.clear();
    h->data[0] = uint256(ans);
    ans.clear();

    for (int i = 0; i < 32; i++)
    {
        ss << setw(2) << setfill('0') << hex << (int)buffer[index++];
    }
    ss >> ans;
    ss.clear();
    h->data[1] = uint256(ans);
    ans.clear();

    int tmp;
    for (int i = 0; i < 4; i++)
    {
        tmp = buffer[index++];
        h->msg_size += (tmp << (3 - i) * 8);
    }
    return h;
}

void send_msg(int sockfd, const msg_header &h, const string &m)
{
    uint8_t *buff = header_buffer(h);
    send(sockfd, buff, 133, 0);
    usleep(100);
    send(sockfd, m.c_str(), m.size(), 0);
}

char recv_header(int sockfd, msg_header &h)
{
    uint8_t buffer[134];
    recv(sockfd, buffer, 134, 0);
    h = *buffer_header(buffer);
    return h.kind;
}
void recv_content(int sockfd, string &m)
{
    char buffer[2048] = {'\0'};
    recv(sockfd, buffer, 2048, 0);
    int index = 0;
    while (buffer[index] != '\0')
    {
        m += buffer[index++];
    }
}

StorageData *recv_file(int fd)
{
}
bool send_file(int fd, StorageData *d)
{
}

//将文件块发送至套接字
bool send_fblock(int fd, FileBlock *d)
{
}

//从文件描述符接收文件块
FileBlock *recv_fblock(int fd)
{
}
#endif