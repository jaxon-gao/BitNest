#pragma once

class WalletManager
{
private:
    map<uint256, coin *> cash;

public:
    //pay num dollar to client
    payment *pay(int num, PeerInfo *client);
    //from contract mint coin
    bool collection(int num, Contract *c);
};