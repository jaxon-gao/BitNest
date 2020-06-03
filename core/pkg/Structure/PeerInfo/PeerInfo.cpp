#include<PeerInfo/PeerInfo.h>
#include<iostream>
#include<vector>
using namespace std;
  PeerInfo::PeerInfo(uint256 hash, int sockfd1)
  {
    h = hash;
    sockfd = sockfd1;
  }
  int PeerInfo::fd()
  {
    return sockfd;
  }
  uint256 PeerInfo::hash()
  {
    return h;
  }
  uint256 PeerInfo::distance(PeerInfo *b)
  {
    //任务：计算哈希异或
    return h^(b->hash());
  }

  void PeerInfo::fd(int sock){
    sockfd=sock;
  }
  //get the same layer number
  uint256 PeerInfo::layer(PeerInfo *b){
      uint256 dis=distance(b);
      uint256 _0=0;
      uint256 lay=0;
      uint256 ful=0;
      uint256 ret=1;
      uint256 an;
      for(int i=0;i<256;i++){
          lay=pow_2((uint256)i);
          ful=_0-lay;
          an=dis&ful;
          if(an!=0&&an==lay&&ret<lay){
              ret=lay;
          }
      }
      return ret;
  }