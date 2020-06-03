#include <contract/test_contract.h>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

bool test_contract(){
    uint256 h = 1024;
    string data;
    data+="1\n";
    data+="2\n";
    data+="3\n";
    data+="4\n";
    data+="5\n";
    data+="6\n";
    data+="7\n";
    data+="8\n";
    BackUp b(h,data);
    if(b.data()!="1024\nbackup\n"+data){
        cerr<<"[err] backup wrong"<<endl;
        return false;
    }
    data+="9\n";
    payment p(h,data);
    if(p.createTime()!="1"){
        cerr<<"[err] payment createTime wrong"<<endl;
        return false;
    }
    if(p.data()!="1024\npayment\n"+data){
        cerr<<"[err] payment wrong"<<endl;
        return false;
    }
    data+="10\n";

    Storage s(h,data);
    if(s.data()!="1024\nstorage\n"+data){
        cerr<<"[err] storage wrong"<<endl;
        return false;
    }

    cout<<"test: contract test finished"<<endl;
    return true;

}