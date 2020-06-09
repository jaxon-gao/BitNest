#include <StoreManager/StorageManager.h>
#include <map>
#include <vector>
using namespace std;
StorageData *StorageManager::search(signature sig, PeerInfo *p)
{
    //添加验证
    if (1 == files.count(sig))
    {
        StorageData *ret = files[sig];
        return ret;
    }
    else
    {

        return NULL;
    }
}
//add file which identified by PeerInfo
void StorageManager::addFile(signature sig, StorageData *d)
{

    if (1 == files.count(sig))
    {
    }
    else
    {
        files[sig] = d;
    }
}