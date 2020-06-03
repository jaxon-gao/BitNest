#ifndef NEST_BLOCK
#define NEST_BLOCK
#include<uint256.h>
#include<string>
#include<sstream>
#include<vector>
#include<map>
#include<time.h>
#include<proof/signature.h>
#include<post/post.h>
#include<FileSystem/NFile.h>
using namespace std;
struct blockHeader{
    uint256 preHash;
    uint256 curHash;
    uint256 merkleRoot;
    time_t timestamp;
    vector<string> data(){
        vector<string> ans;
        stringstream ss;
        string tmp;
        ss<<hex<<preHash;
        ss>>tmp;
        ans.push_back(tmp);

        ss<<hex<<curHash;
        ss>>tmp;
        ans.push_back(tmp);

        ss<<hex<<merkleRoot;
        ss>>tmp;
        ans.push_back(tmp);

        ss<<timestamp;
        ss>>tmp;
        ans.push_back(tmp);

        return ans;
    }
    blockHeader(vector<string> v){
        preHash=uint256(v[0]);
        curHash=uint256(v[1]);
        merkleRoot=uint256(v[2]);
        timestamp=atoi(v[3].c_str());
    }
};

class block :public NFile{
public:
    block(uint256 h,string kind);
    block();
    block(block & b);
    string kind;
    uint256 hash();
    bool header(blockHeader *data);
    bool header(vector<string> data);
    blockHeader* header();

    bool ElectionRecord(vector<signature*> sigs);
    vector<signature*> ElectionRecord();

    bool Signature(vector<signature*> sigs);
    vector<signature*> Signature();

    //打包者哈希
};
#endif