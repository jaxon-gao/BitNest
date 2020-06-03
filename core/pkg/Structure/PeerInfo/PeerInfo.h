// Copyright (c) 2020-2020 Jaxon Gao
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef NEST_PEERINFO_
#define NEST_PEERINFO_
#include "uint256.h"

class PeerInfo
{
private:
  int sockfd;
  uint256 h;
public:
  uint256 pk[2];
  PeerInfo(uint256 hash, int sockfd1);
  int fd();
  void fd(int sock);
  uint256 hash();
  uint256 distance(PeerInfo *b);
  uint256 layer(PeerInfo *b);
};
#endif