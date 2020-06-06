#ifndef NEST_NET_TOOLS
#define NEST_NET_TOOLS
#include <tools/net_tool.h>
#include <msg/msg.h>
#include <stdio.h>
#include <uint256.h>
#include <vector>
#include <config.h>
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
#include <fstream>
#include <FileSystem/StorageData.h>
#include <FileSystem/FileBlock.h>
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

//将文件块发送至套接字
bool send_fblock(int fd, FileBlock *d)
{
    //发送本块大小
    char buff[1024];
    send(fd, (void *)&d->size, 4, 0);

    vector<uint8_t> bits = d->hash.export_bits();
    for (int i = 0; i < 32; i++)
    {
        buff[i] = bits[i];
    }
    //发送哈希
    send(fd, buff, 32, 0);

    //发送ID和块总数量
    send(fd, (void *)&d->ID, 4, 0);
    FILE *f = fopen(d->path.c_str(), "r");
    int r;
    while (1)
    {
        r = fread(buff, 1024, 1, f);
        r = strlen(buff);
        if (r <= 0)
        {
            break;
        }
        send(fd, buff, r, 0);
    }
}

//从文件描述符接收文件块
FileBlock *recv_fblock(int fd)
{
    //接收文件大小
    char buff[1024];
    FileBlock *ans = new FileBlock;
    recv(fd, (void *)&ans->size, 4, 0);
    recv(fd, (void *)buff, 32, 0);

    //接收哈希，形成本地路径
    stringstream ss;
    string hash_str;
    for (int i = 0; i < 32; i++)
    {
        ss << setw(2) << setfill('0') << hex << (int)buff[i];
    }
    ss >> hash_str;
    ans->hash = uint256(hash_str);
    string path = root_path + "/files";
    string cmd = "mkdir -p " + path;
    system(cmd.c_str());
    ans->path = path + "/" + hash_str;

    //接收ID
    recv(fd, (void *)&ans->ID, 4, 0);

    ofstream ofs;
    ofs.open(ans->path.c_str());
    int read;
    while (1)
    {
        read = recv(fd, buff, 1024, 0);
        if (read <= 0)
        {
            break;
        }
        ofs.write(buff, read);
    }
    return ans;
}

StorageData *recv_file(int fd)
{
    uint8_t buff[64];
    StorageData *ans = new StorageData;
    recv(fd, buff, 32, 0);
    stringstream ss;
    string str;
    for (int i = 0; i < 32; i++)
    {
        ss << setw(2) << setfill('0') << hex << (int)buff[i];
    }
    ss >> str;
    ans->root_hash = uint256(str);
    recv(fd, (void *)&ans->BlockNum, 4, 0);
    int size;
    recv(fd, (void *)&size, 4, 0);
    FileBlock *tmp;
    for (int i = 0; i < size; i++)
    {
        tmp = recv_fblock(fd);
        ans->files.push_back(tmp);
        ans->IDs.insert(tmp->ID);
    }
    return ans;
}
bool send_file(int fd, StorageData *d)
{
    uint8_t buff[64];
    vector<uint8_t> bits = d->root_hash.export_bits();
    for (int i = 0; i < 32; i++)
    {
        buff[i] = bits[i];
    }
    int size = d->files.size();
    send(fd, buff, 32, 0);
    send(fd, (void *)&d->BlockNum, 4, 0);
    send(fd, (void *)&size, 4, 0);
    for (int i = 0; i < size; i++)
    {
        send_fblock(fd, d->files[i]);
    }
    return true;
}

#endif