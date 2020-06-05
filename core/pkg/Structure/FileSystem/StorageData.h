#pragma once
#include <string>
#include <uint256.h>
#include <FileSystem/FileBlock.h>
#include <vector>
#include <set>
using namespace std;
//存储完整文件或一部分
struct StorageData
{
    uint256 root_hash;
    //完整文件的快数量
    int BlockNum;
    vector<FileBlock *> files;
    //默克尔ID
    set<int> IDs;
};