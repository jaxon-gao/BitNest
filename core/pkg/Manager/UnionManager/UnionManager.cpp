#include <UnionManager/UnionManager.h>
#include <sstream>
#include <hash/sha256.h>
#include <time.h>
#include <algorithm>
#include <stdlib.h>
#include <config.h>
#include <unistd.h>
#include <iomanip>
UnionManager::UnionManager() {}
UnionManager::UnionManager(KeyPair *k) {key=k;};
bool UnionManager::Offline(PeerInfo *p)
{
  //使用折半查找优化？
  for (int i = 0; i < connection.size(); i++)
  {
    if (connection[i]->hash() == p->hash())
    {
      close(connection[i]->fd());
      delete connection[i];
      connection.erase(connection.begin() + i, connection.begin() + i + 1);
      return true;
    }
  }
  return false;
}
bool UnionManager::AddNode(PeerInfo *p)
{
  Offline(p);
  connection.push_back(p);
}

uint256 gethash(PeerInfo *p, time_t t)
{
  sha256_t hash_cnt;
  string ans, hash_str;
  stringstream ss;
  uint8_t hash_output[32];
  ss << hex << p->pk[0];
  ss << hex << p->pk[1];
  ss << hex << p->hash();
  ss << hex << t;
  ss >> ans;
  sha256_init(&hash_cnt);
  sha256_update(&hash_cnt, (unsigned char *)ans.c_str(), 96);
  sha256_final(&hash_cnt, hash_output);
  stringstream sn;
  for (int i = 0; i < 32; i++)
  {
    sn << hex << setw(2) << setfill('0') << (unsigned int)hash_output[i];
  }
  hash_str.clear();
  sn >> hash_str;
  uint256 hash_num(hash_str);
  return hash_num;
}

PeerInfo *UnionManager::Election()
{
  time_t curr_ = time(0) / 45;
  time_t last_ = last / 45;
  if (curr_ - last_ == 0)
  {
    return leader;
  }
  PeerInfo *l = NULL;
  uint256 max = 0;
  for (int i = 0; i < connection.size(); i++)
  {
    uint256 hash_num = gethash(connection[i], curr_);
    if (hash_num > max)
    {
      max = hash_num;
      l = connection[i];
    }
  }
  leader = l;
  return leader;
}
struct union_depart_helper
{
  uint256 hash;
  PeerInfo *peer;
};
bool helper_compare(union_depart_helper a, union_depart_helper b)
{
  return a.hash > b.hash;
}

bool UnionManager::Depart()
{
  time_t curr_ = time(0);
  if (connection.size() < MAX_UNION_SIZE)
  {
    return false;
  }
  if (time(0) % 45 > 40)
  {
    return false;
  }
  else
  {
    union_depart_helper helper;
    vector<union_depart_helper> UnionHash;
    for (int i = 0; i < connection.size(); i++)
    {
      uint256 hash_num = gethash(connection[i], curr_);
      helper.hash = hash_num;
      helper.peer = connection[i];
      UnionHash.push_back(helper);
    }
    uint256 myhash = gethash(my, curr_);
    if (myhash > UnionHash[UnionHash.size() / 2].hash)
    {
      for (int i = UnionHash.size() / 2; i < UnionHash.size(); i++)
      {
        Offline(UnionHash[i].peer);
      }
    }
    else
    {
      for (int i = 0; i <= UnionHash.size() / 2; i++)
      {
        Offline(UnionHash[i].peer);
      }
      Election();
    }
  }
}
bool UnionManager::Emit(const string &str)
{
}

void *UnionManager::service_union(void *args)
{
  while (1)
  {
    sleep(1);
    Election();
    Depart();
  }
}

bool UnionManager::isUnionLeader()
{
  if (my->hash() == leader->hash())
  {
    return true;
  }
  else
    return false;
}
//TODO：使用折半查找优化
bool UnionManager::HasNode(PeerInfo *p)
{
  for (int i = 0; i < connection.size(); i++)
  {
    if (p->hash() == connection[i]->hash())
    {
      return true;
    }
  }
  return false;
}

vector<PeerInfo *> UnionManager::getAll()
{
  return connection;
}