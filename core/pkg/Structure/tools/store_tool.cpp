#include <tools/store_tool.h>
using namespace std;
//分块，解决临时块存放问题
StorageData *depart_file(string path)
{
    FILE *f = fopen(path.c_str(), "r");
    uint8_t buff[1024];
    int r;
    int size = 0;
    string path_tmp = path + ".tmp";
    string path_curr;
    ofstream ofs;
    stringstream ss;
    string hash_str;
    ofs.open(path_tmp.c_str());
    StorageData *ret = new StorageData;
    while (r = fread(buff, 1, 1024, f))
    {
        ofs.write(buff, r);
        size++;
        if (size == 64 || feof(f))
        {
            FileBlock *n = new FileBlock;
            uint256 hash = getHash(path_tmp);
            ss.clear();
            ss << hex << hash;
            ss >> hash_str;
            copy(path_tmp, n->path);
            ofs.open(path_tmp.c_str());
            n->hash = hash;
            n->path = root_path + "/file/" + hash_str;
            n->size = size;

            ret->files.push_back(n);
        }
    }
    fclose(f);
    merkle *m = new merkle(ret);
    delete m;
    return ret;
}

void copy(string from, string to)
{
    char buff[1024];
    FILE *f1 = fopen(from.c_str(), "r");
    FILE *f2 = fopen(to.c_str(), "w");
    int r;
    while (r = fread(buff, 1, 1024, f1))
    {
        fwrite(buff, 1, r, f2);
    }
    fclose(f1);
    fclose(f2);
}