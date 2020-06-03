#ifndef NEST_UMANAGER
#define NEST_UMANAGER
#include <vector>
#include <KeyPair/KeyPair.h>
#include <PeerInfo/PeerInfo.h>
using namespace std;
class UnionManager
{
private:
    vector<PeerInfo *> connection;
    PeerInfo *leader;
    PeerInfo *my;
    KeyPair *key;
    vector<uint256> UnionIDs;
    time_t last;

public:
    UnionManager(KeyPair *k);
    bool Offline(PeerInfo *p);
    bool AddNode(PeerInfo *p);
    PeerInfo *Election(time_t t);
    bool Depart();
    bool Emit(const string &str);
    void *service_union(void *args);
    bool isUnionLeader();
    void daemon();
};
#endif