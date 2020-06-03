#ifndef NEST_USER_OVERSHOT
#define NEST_USER_OVERSHOT
#include<PeerInfo/PeerInfo.h>
#include<contract/payment.h>
#include<contract/storage.h>
#include<contract/backup.h>
#include<vector>
using namespace std;
class UserOvershot{
public:
    UserOvershot(string data);
    UserOvershot();
    uint256 h;
    uint256 cash;
    uint256 frezon;
    vector<Contract*> contracts;
    string data();
};
#endif