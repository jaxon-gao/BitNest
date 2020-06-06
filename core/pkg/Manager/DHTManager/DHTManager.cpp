// Copyright (c) 2020-2020 Jaxon Gao
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include <DHTManager/DHTManager.h>
#include <vector>
#include <sys/epoll.h>
using namespace std;
DHTManager::DHTManager(uint256 hash, int e)
{
    epoll_in = e;
    my = new PeerInfo(hash, 0);
}

bool DHTManager::AddNode(PeerInfo *p)
{
    uint256 lay = my->layer(p);
    if (1 == buckets.count(lay))
    {
        buckets[lay]->AddNode(p, my);
    }
    else
    {
        buckets[lay] = new Bucket(lay);
        buckets[lay]->AddNode(p, my);
    }
    return true;
}
bool DHTManager::DelNode(PeerInfo *p)
{
    uint256 lay = my->layer(p);
    if (1 == buckets.count(lay))
    {
        return buckets[lay]->DelNode(p);
    }
    else
    {
        return false;
    }
}
vector<PeerInfo *> DHTManager::Query(PeerInfo *p)
{
    vector<PeerInfo *> ans;
    map<uint256, Bucket *>::iterator iter = buckets.begin();
    Bucket *b;
    while (iter != buckets.end())
    {
        b = iter->second;
        ans.push_back(b->findNode(p));
        iter++;
    }
    return ans;
}
vector<PeerInfo *> DHTManager::GetRandomNode()
{
    vector<PeerInfo *> ans;
    map<uint256, Bucket *>::iterator iter = buckets.begin();
    Bucket *b;
    set<PeerInfo *> tmp;
    while (iter != buckets.end())
    {
        b = iter->second;
        tmp = b->getRandomPeer();
        ans.insert(ans.end(), tmp.begin(), tmp.end());
        iter++;
    }
    return ans;
}
vector<PeerInfo *> DHTManager::GetAllNode()
{
    vector<PeerInfo *> ans;
    map<uint256, Bucket *>::iterator iter = buckets.begin();
    Bucket *b;
    vector<PeerInfo *> tmp;
    while (iter != buckets.end())
    {
        b = iter->second;
        tmp = b->getAllNode();
        ans.insert(ans.end(), tmp.begin(), tmp.end());
        iter++;
    }
    return ans;
}
//连接至DHT网络
void DHTManager::connect()
{
}
void DHTManager::connect(PeerInfo *seed)
{
}

bool DHTManager::OldNode(PeerInfo *p)
{
    uint256 lay = my->layer(p);
    if (1 == buckets.count(lay))
    {
        return buckets[lay]->OldNode(p);
    }
    else
    {
        return false;
    }
}

void DHTManager::EpollDel(PeerInfo *p)
{
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = p->fd();
    epoll_ctl(epoll_in, EPOLL_CTL_DEL, ev.data.fd, &ev);
}