#include<Structure/FileSystem/test_nfile.h>
#include<Structure/FileSystem/NFile.h>
#include<string>
#include<iostream>
#include <string.h>
using namespace std;
bool test_nfile(){
    string path01="file1.txt";
    string path02="/file2.txt";
    NFile f1(path01);
    uint256 hash = 1024;
    NFile f2(hash,"contract");
    f1.write("Jaxon","Gao");
    f1.write("Jaxon1","Gao");
    string get1 = f1.get("Jaxon");
    if(strcmp(get1.c_str(),"Gao")!=0){
        cout<<"get or write wrong"<<endl;
        return false;
    }
    f1.remove("Jaxon");
    if(strcmp(f1.get("Jaxon").c_str(),"Gao")==0){
        cout<<"remove error"<<endl;
        return false;
    }

    vector<string> val;
    val.push_back("111");
    val.push_back("222");
    val.push_back("333");
    f2.write("nums",val);
    string data="";
    data+="111\n";
    data+="222\n";
    data+="333\n";
    if(f2.get("nums")!=data){
        return false;
    }
    data="222\n333\n";
    f2.remove("nums","111");;
    if(f2.get("nums")!=data){
        return false;
    }
    f2.remove("nums","111");    
    if(f2.get("nums")!=data){
        return false;
    }

    f2.remove("nums");
    if(f2.get("nums").size()!=0){
        return false;
    }
    cout<<"test: Nfile test finished"<<endl;
    return true;
}