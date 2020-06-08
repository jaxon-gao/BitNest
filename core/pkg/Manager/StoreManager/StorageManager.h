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
    map<signature, StorageData *> files;

public:
    StorageManager() {}
    //using PeerInfo to search
    StorageData *search(signature sig, PeerInfo *p);
    //add file which identified by PeerInfo
    void addFile(signature sig, StorageData *d);
};