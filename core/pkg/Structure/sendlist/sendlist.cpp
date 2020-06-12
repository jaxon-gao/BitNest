#include <sendlist/sendlist.h>
#include <iostream>
using namespace std;
sendlist::sendlist(int e)
{
    epoll_fd = e;
    sem_lock = semget((key_t)epoll_fd * 102, 1, 0666 | IPC_CREAT);
    set_semvalue(sem_lock, 1);
}
void sendlist::new_msg(int fd, msg m)
{
    lock_p(sem_lock);
    if (1 == list_msg.count(fd))
    {
        list_msg[fd].push_back(m);
    }
    else
    {
        vector<msg> vv;
        vv.push_back(m);
        list_msg[fd] = vv;
    }
    lock_v(sem_lock);
}
vector<msg> sendlist::msg_of(int fd)
{
    lock_p(sem_lock);
    vector<msg> vv;
    if (1 == list_msg.count(fd))
    {
        vector<msg> v2 = list_msg[fd];
        list_msg[fd] = vv;
        return v2;
    }
    else
    {
        return vv;
    }
    lock_v(sem_lock);
}

vector<msg> sendlist::msg_failed(int fd, vector<msg> vv)
{
    lock_p(sem_lock);
    if (1 == list_msg.count(fd))
    {
        for (int i = 0; i < vv.size(); i++)
        {
            list_msg[fd].push_back(vv[i]);
        }
    }
    else
    {
        list_msg[fd] = vv;
    }

    lock_v(sem_lock);
}

bool sendlist::has_msg(int fd)
{
    lock_p(sem_lock);
    if (1 == list_msg.count(fd))
    {
        if (list_msg[fd].size() == 0)
        {
            lock_v(sem_lock);
            return false;
        }
        lock_v(sem_lock);
        return true;
    }
    else
    {
        lock_v(sem_lock);
        return false;
    }
}

sendlist::~sendlist()
{
    del_semvalue(sem_lock);
}

vector<int> sendlist::get_fd()
{
    vector<int> ans;
    map<int, vector<msg>>::iterator iter = list_msg.begin();
    while (iter != list_msg.end())
    {
        int fd = iter->first;
        if (has_msg(fd))
        {
            ans.push_back(fd);
        }
        iter++;
    }
    return ans;
}
