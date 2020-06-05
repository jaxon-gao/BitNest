#pragma once

#include <string>
#include <uint256.h>
using namespace std;
struct FileBlock
{
    uint32_t size;
    //文件根哈希
    uint256 hash_root;
    //本文件块哈希
    uint256 hash;
    //在所属文件中的merkle ID
    int ID;
    //所属文件块数量
    int blockNum;
    string path;
};