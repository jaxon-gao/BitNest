// Copyright (c) 2020-2020 Jaxon Gao
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef NEST_BUCKET
#define NEST_BUCKET
#include <PeerInfo/PeerInfo.h>
#include <uint256.h>
#include <vector>
#include <set>
using namespace std;
class Bucket
{
private:
  uint256 layer;
  vector<PeerInfo *> peers;

public:
  Bucket(uint256 dis);
  //add new node, if bucket is full return false
  bool AddNode(PeerInfo *p, PeerInfo *My);
  //del node which hash equals p.hash();
  bool DelNode(PeerInfo *p);
  bool OldNode(PeerInfo *p);

  set<PeerInfo *> getRandomPeer();

  vector<PeerInfo *> getAllNode();

  PeerInfo *findNode(PeerInfo *p);
};
#endif