#ifndef NEST_NET_TOOLS
#define NEST_NET_TOOLS

#include <msg/msg.h>
#include<string>
using namespace std;
int setnonblocking( int fd );
void setreuseaddr(int sock);

uint8_t* header_buffer(const msg_header&h);
msg_header* buffer_header(uint8_t *buffer);
void send_msg(int sockfd, const msg_header &h,const string & m);
char recv_header(int sockfd, msg_header &h);
void recv_content(int sockfd, string & m);
#endif