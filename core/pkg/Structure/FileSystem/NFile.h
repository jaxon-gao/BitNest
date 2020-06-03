
#ifndef NEST_FILE
#define NEST_FILE
#include <string>
#include <nlock.h>
#include <uint256.h>
#include <string>
#include <iostream>
//Nest File Unit
using namespace std;
class NFile{
    public:
        NFile(string path);
        NFile(string path,bool erase);
        NFile(uint256 hash, string kind);
        NFile(uint256 hash, string kind, bool erase);
        ~NFile();
        int sem;
        string path;
        string absolutePath;
        bool write(string key,string value);
        bool write(string key,vector<string> value);
        bool add(string key,vector<string> value);
        bool add(string key,string value);
        string get(string key);
        vector<string> gtv(string key);
        FILE *file;
        void lock();
        void release();
        bool remove(string key);
        bool remove(string key,string val);
    private:
        bool format();
};
#endif