#include <DHTManager/test_bucket.h>
#include <DHTManager/Bucket.h>
#include <iostream>
using namespace std;
bool test_bucket(){
    //test add and delete
    Bucket b1(pow_2(3));
    PeerInfo *my=new PeerInfo(pow_2(3)+1,0);
    PeerInfo *_0000=new PeerInfo(0,0);
    PeerInfo *_0001=new PeerInfo(1,0);
    PeerInfo *_0100=new PeerInfo(pow_2(3),0);
    PeerInfo *_1000=new PeerInfo(pow_2(4),0);
    if(!b1.AddNode(_0000,my)){
        return false;
    }
    if(!b1.AddNode(_0001,my)){
        return false;
    }
    if(b1.AddNode(_0100,my)){
        return false;
    }
    if(b1.AddNode(_1000,my)){
        return false;
    }
    if(b1.DelNode(_1000)){
        return false;
    }
    if(!b1.DelNode(_0001)){
        return false;
    }
    //test Random Peer
    Bucket b2(pow_2(10));
    PeerInfo *pointers[100];
    for(int i=0;i<100;i++){
        pointers[i]=new PeerInfo(1024+i,0);
        b2.AddNode(pointers[i],my);
    }
    set<PeerInfo *> ran = b2.getRandomPeer();
    if(ran.size()<4){
        return false;
    }
    PeerInfo*tmp;
    set<PeerInfo *>::iterator iter=ran.begin();
    
    //check find node
    PeerInfo * fn = b2.findNode(pointers[0]);
    if(fn->hash()!=1024){
        return false;
    }
    fn = b2.findNode(my);
    if(fn->hash()!=1041){
        return false;
    }
    cout<<"test: bucket test run finished"<<endl;
    return true;
}