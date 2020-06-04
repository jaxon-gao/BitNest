#ifndef NEST_UMANAGER
#define NEST_UMANAGER
#include <vector>
#include <KeyPair/KeyPair.h>
#include <PeerInfo/PeerInfo.h>
#include <uint256.h>
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
    int epoll_in;

public:
    UnionManager(int e);
    UnionManager(KeyPair *k);
    bool Offline(PeerInfo *p);
    bool EpollDel(PeerInfo *p);
    bool AddNode(PeerInfo *p);
    bool HasNode(PeerInfo *p);
    PeerInfo *Election();
    bool Depart();
    bool Emit(const string &str);
    void *service_union(void *args);
    bool isUnionLeader();
    void daemon();
    vector<PeerInfo *> getAll();
};
#endif