// Copyright (c) 2020-2020 Jaxon Gao
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <algorithm>
#include <ctime>
#include <DHTManager/Bucket.h>
#include <config.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;
bool PeerInfoCom(PeerInfo *a, PeerInfo *b)
{
    return a->hash() > b->hash();
}

Bucket::Bucket(uint256 dis)
{
    layer = dis;
}

bool Bucket::AddNode(PeerInfo *p, PeerInfo *My)
{
    if (peers.size() >= MAX_BUCKET_SIZE)
    {
        return false;
    }
    uint256 dis;
    dis = p->distance(My);
    uint256 tmp = 0;
    tmp -= layer;
    uint256 _3 = tmp & dis;
    if (_3 == layer)
    {
        peers.push_back(p);
        sort(peers.begin(), peers.end(), PeerInfoCom);
        if (peers.size() >= MAX_BUCKET_SIZE)
        {
            close(peers[peers.size() - 1]->fd());
            delete peers[peers.size() - 1];
            peers.erase(peers.begin() + peers.size() - 1);
        }
        return true;
    }
    return false;
}

bool Bucket::DelNode(PeerInfo *p)
{
    if (p->hash() == 0)
    {
        for (int i = 0; i < peers.size(); i++)
        {
            if (p->fd() == peers[i]->fd())
            {
                delete peers[i];
                peers.erase(peers.begin() + i);
                return true;
            }
        }
    }
    else
    {
        for (int i = 0; i < peers.size(); i++)
        {
            if (peers[i]->hash() == p->hash())
            {
                delete peers[i];
                peers.erase(peers.begin() + i);
                return true;
            }
        }
    }
    return false;
}

set<PeerInfo *> Bucket::getRandomPeer()
{
    srand(unsigned(time(0)));
    int ran = rand();
    int step;
    set<PeerInfo *> ret;
    if (peers.size() < 5)
    {
        for (int i = 0; i < peers.size(); i++)
        {
            ret.insert(peers[i]);
        }
    }
    else if (peers.size() < 50)
    {
        step = 10;
    }
    else if (peers.size() < 100)
    {
        step = 20;
    }
    else
    {
        step = 40;
    }
    int k = 10;
    while (k--)
    {
        ret.insert(peers[(ran + k * step) % peers.size()]);
    }
    return ret;
}

vector<PeerInfo *> Bucket::getAllNode()
{
    return peers;
}

bool Bucket::OldNode(PeerInfo *p)
{
    //使用折半查找优化

    for (int i = 0; i < peers.size(); i++)
    {
        if (p->hash() == peers[i]->hash())
        {
            return true;
        }
    }

    return false;
}
PeerInfo *Bucket::findNode(PeerInfo *p)
{
    PeerInfo *ret = peers[0];
    uint256 dis = p->distance(peers[0]);
    for (int i = 0; i < peers.size(); i++)
    {
        if (dis > p->distance(peers[i]))
        {
            dis = p->distance(peers[i]);
            ret = peers[i];
        }
    }
    return ret;
}