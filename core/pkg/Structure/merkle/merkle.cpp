#include <merkle/merkle.h>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

uint256 getHash(string path)
{
    uint8_t buff[1024] = {0};
    FILE *f = fopen(path.c_str(), "rb");

    sha256_t hash_cnt;
    sha256_init(&hash_cnt);
    int r = 1;
    while (r = fread(buff, 1, 1024, f))
    {
        sha256_update(&hash_cnt, (unsigned char *)buff, r);
    }
    fclose(f);
    return getHash(hash_cnt);
}

uint256 getHash(const char *path)
{
    uint8_t buff[1024] = {0};
    FILE *f = fopen(path, "rb");
    sha256_t hash_cnt;
    sha256_init(&hash_cnt);
    int r = 1;
    while (r = fread(buff, 1, 1024, f))
    {
        sha256_update(&hash_cnt, (unsigned char *)buff, r);
    }
    fclose(f);
    return getHash(hash_cnt);
}
uint256 getHash(vector<uint8_t> v)
{
    uint8_t hash_output[32];
    sha256_t hash_cnt;
    uint8_t *buff = new uint8_t[v.size()];
    for (int i = 0; i < v.size(); i++)
    {
        buff[i] = v[i];
    }
    sha256_init(&hash_cnt);
    sha256_update(&hash_cnt, (unsigned char *)buff, v.size());
    delete buff;
    return getHash(hash_cnt);
}

uint256 getHash(sha256_t hash_cnt)
{
    uint8_t hash_output[32];
    sha256_final(&hash_cnt, hash_output);
    stringstream ss;
    string str;
    for (int i = 0; i < 32; i++)
    {
        ss << setw(2) << setfill('0') << hex << (int)hash_output[i];
    }
    ss >> str;
    return uint256(str);
}

uint256 getHash(uint256 int256)
{
    vector<uint8_t> bits = int256.export_bits();
    return getHash(bits);
}

uint256 getHash(uint256 param1, uint256 param2)
{
    vector<uint8_t> b1 = param1.export_bits();
    vector<uint8_t> b2 = param2.export_bits();
    for (int i = 0; i < b2.size(); i++)
    {
        b1.push_back(b2[i]);
    }
    return getHash(b1);
}

merkle::merkle(StorageData *d)
{
    status = false;
    FILE *f;
    vector<FileBlock *> *blocks = &d->files;
    uint8_t buff[1024];
    int r;
    sha256_t hash_cnt;
    for (int i = 0; i < blocks->size(); i++)
    {
        d->IDs.insert(i);
        (*blocks)[i]->ID = i;
        sha256_init(&hash_cnt);
        f = fopen((*blocks)[i]->path.c_str(), "r");
        r = 1;
        while (r > 0)
        {
            r = fread(buff, 1, 1024, f);
            sha256_update(&hash_cnt, buff, r);
        }
        uint256 h_num = getHash(hash_cnt);
        tree.push_back(h_num);
    }
    generate();
    d->root_hash = root;
    for (int i = 0; i < blocks->size(); i++)
    {
        d->root_hash = root;
        d->BlockNum = blocks->size();
    }
}

bool merkle::addleaf(uint256 hash)
{
    if (!status)
    {
        tree.push_back(hash);
    }
    else
    {
        vector<uint256> tree_new;
        int size = tree.size();
        for (int i = 0; i < size; i++)
        {
            tree_new.push_back(tree[size / 2 + i]);
        }
        tree_new.push_back(hash);
        status = false;
        generate();
    }
}

bool merkle::generate()
{
    if (tree.size() == 0)
    {
        return false;
    }
    else if (!status)
    {
        if (tree.size() == 1)
        {
            status = true;
            root = tree[0];
            return true;
        }
        int size = tree.size();
        int l;
        int k = 2;
        while (k < size)
        {
            k *= 2;
        }
        for (int i = 0; i < k - size; i++)
        {
            tree.push_back(0);
        }
        vector<uint256> tree_new(k * 2 - 1);
        for (int i = 0; i < k; i++)
        {
            tree_new[i + k] = tree[i];
        }
        for (int i = tree.size() - 2; i > 0; i -= 2)
        {
            tree_new[i / 2] = getHash(tree[i], tree[i + 1]);
        }
        tree = tree_new;
        status = true;
    }
    else
    {
        return false;
    }
}

bool merkle::addleaf(vector<uint8_t> leafdata)
{
    uint256 hash = getHash(leafdata);
    return addleaf(hash);
}

uint256 merkle::hash_root()
{
    return root;
}