#pragma once
#include <set>
#include <map>
#include <vector>
#include <msg/msg.h>
using namespace std;
class sendlist
{
private:
    //监听集合
    set<int> sender_listen;
    //消息队列
    map<int, vector<msg>> sendlist;
    int epoll_fd;

public:
    sendlist(int e);
    void new_msg(msg m);
    vector<msg> msg_of(int fd);
};