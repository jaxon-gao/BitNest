#pragma once
#include <string>
#include <vector>
#include <map>
#include <FileSystem/StorageData.h>
using namespace std;
class StorageManager
{
private:
    //PeerOnfo
    map<signature, vector<StorageData *>> files;

public:
    //using PeerInfo to search
    StorageData *search(uint256 hash, PeerInfo *p);
    //add file which identified by PeerInfo
    void addFile(signature sig, StorageData *d);
};