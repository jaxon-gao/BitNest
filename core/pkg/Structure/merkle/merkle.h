#pragma once
#include <hash/sha256.h>
#include <queue>
#include <uint256.h>
#include <FileSystem/StorageData.h>
using namespace std;
//get hash from byte vector
uint256 getHash(vector<uint8_t> v);
uint256 getHash(sha256_t hash_cnt);
uint256 getHash(uint256 int256);
uint256 getHash(uint256 param1, uint256 param2);
uint256 getHash(string path);
uint256 getHash(const char *path);
class merkle
{
private:
    bool status;
    vector<uint256> tree;
    uint256 root;

public:
    merkle() {}
    merkle(StorageData *d);
    bool addleaf(vector<uint8_t> leafdata);
    bool addleaf(uint256 hash);
    bool generate();
    uint256 hash_root();
};