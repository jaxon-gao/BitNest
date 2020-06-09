#pragma once
#include <set>
#include <map>
#include <vector>
#include <msg/msg.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <nlock.h>
using namespace std;
class sendlist
{
private:
    //监听集合
    set<int> sender_listen;
    //消息队列
    map<int, vector<msg>> list_msg;
    int epoll_fd;
    int sem_lock;

public:
    sendlist(int e);
    void new_msg(int fd, msg m);
    vector<msg> msg_of(int fd);
    vector<msg> msg_failed(int fd, vector<msg> vv);
    bool has_msg(int fd);
    vector<int> get_fd();
    ~sendlist();
};