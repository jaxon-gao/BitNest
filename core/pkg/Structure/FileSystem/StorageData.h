#pragma once
#include <string>
#include <uint256.h>
#include <FileSystem/FileBlock.h>
using namespace std;

class StorageData
{
public:
  uint256 hash;
  string path;
  FileBlock *content;
  int size;
};