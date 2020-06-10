#pragma once
#include <string>
#include <uint256.h>
#include <FileSystem/FileBlock.h>
#include <vector>
#include <set>
#include <proof/signature.h>
using namespace std;
//存储完整文件或一部分
struct StorageData
{
  uint256 sig[2];
  uint256 root_hash;
  //完整文件的快数量
  int BlockNum;
  vector<FileBlock *> files;
  //默克尔树叶ID
  set<int> IDs;
};