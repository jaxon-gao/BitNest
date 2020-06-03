// Copyright (c) 2020-2020 Jaxon Gao
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef NEST_DHTMANAGER
#define NEST_DHTMANAGER
#include <uint256.h>
#include <vector>
#include <PeerInfo/PeerInfo.h>
#include <DHTManager/Bucket.h>
#include <map>
using namespace std;
class DHTManager
{
private:
    map<uint256, Bucket *> buckets;
    PeerInfo *my;

public:
    DHTManager(uint256 hash);
    void connect();
    void connect(PeerInfo *seed);
    bool AddNode(PeerInfo *p);
    bool OldNode(PeerInfo *p);
    bool DelNode(PeerInfo *p);
    vector<PeerInfo *> Query(PeerInfo *p);
    vector<PeerInfo *> GetRandomNode();
    vector<PeerInfo *> GetAllNode();
};
#endif