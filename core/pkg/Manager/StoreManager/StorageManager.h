#pragma once
#include <string>
#include <vector>
#include <map>
#include <FileSystem/StorageData.h>
using namespace std;
class StorageManager
{
private:
    map<uint256, vector<StorageData *>> files;

public:
    StorageData *search(uint256 hash, PeerInfo *p);
    void addFile(StorageData *file);
};