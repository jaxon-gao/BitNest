#include <uint256/test_uint256.h>
#include <DHTManager/test_bucket.h>
#include <PeerInfo/test_Peerinfo.h>
#include <FileSystem/test_nfile.h>
#include <contract/test_contract.h>
#include <proof/test_signature.h>
#include <mdns/test_mdns.h>
#include <post/test_post.h>
#include <NetManager/test_NetManager.h>
#include <tools/test_net_tool.h>
#include <merkle/test_merkel.h>
#include <iostream>
using namespace std;
int main()
{
    if (!test_nfile())
    {
        cerr << "[err] nfile error" << endl;
    }
    if (!test_uint256())
    {
        cerr << "[err] uint256 error" << endl;
    }
    if (!test_contract())
    {
        cerr << "[err] contract error" << endl;
    }
    if (!test_peerinfo())
    {
        cerr << "[err] peerinfo error" << endl;
    }
    if (!test_bucket())
    {
        cerr << "[err] bucket error" << endl;
    }
    if (!test_signature())
    {
        cerr << "[err] signature error" << endl;
    }
    if (!test_post())
    {
        cerr << "[err] post error" << endl;
    }
    if (!test_net_manager())
    {
        cerr << "[err] NetManager error" << endl;
    }
    if (!test_net_tool())
    {
        cerr << "[err] NetTool error" << endl;
    }
    if (!test_merkle())
    {
        cerr << "[err] merkle error" << endl;
    }

    //output of test passed
    cout << "test_passed" << endl;
}