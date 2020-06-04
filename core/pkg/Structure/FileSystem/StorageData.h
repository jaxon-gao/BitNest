#pragma once
#include <string>
#include <uint256.h>
#include <FileSystem/FileBlock.h>
#include <vector>
using namespace std;

class StorageData
{
public:
    uint256 hash;
    string path;
    vector<FileBlock *> files;
};