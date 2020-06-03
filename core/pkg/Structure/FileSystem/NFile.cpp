#include <FileSystem/NFile.h>
#include <numeric>
#include <fstream>
#include <nlock.h>
#include <iostream>
#include <config.h>
#include <string.h>
#include <string>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <Structure/uint256.h>
#include <sstream>
using namespace std;
NFile::NFile(string path){
    string currPath;
    if(path[0]!='/'){
        currPath=root_path+"/"+path;
    }
    else{
        currPath=root_path+path;
    }
    absolutePath = currPath;
    if (FILE *file = fopen(absolutePath.c_str(), "r")) {
        fclose(file);
    } else {
        ofstream F;
        F.open(absolutePath);
        F.close();
    }
    if(!format()){
        throw "文件格式错误";
    }
    int k=rand();
    sem=semget((key_t)k,1,0666|IPC_CREAT);
    set_semvalue(sem,1);
}
NFile::NFile(string path,bool erase){
    string currPath;
    if(path[0]!='/'){
        currPath=root_path+"/"+path;
    }
    else{
        currPath=root_path+path;
    }
    absolutePath = currPath;
    ofstream F;
    F.open(absolutePath);
    F.close();
    if(!format()){
        throw "文件格式错误";
    }
    int k=rand();
    sem=semget((key_t)k,1,0666|IPC_CREAT);
    set_semvalue(sem,1);
}

NFile::NFile(uint256 hash, string kind){
    //创建文件夹
    string cmd = "mkdir -p " + root_path+"/"+kind; 
    system(cmd.c_str());
    string str;
    stringstream ss;
    ss<<hex<<hash;
    ss>>str;
    path = str+"."+kind;
    absolutePath=root_path+"/"+kind+"/"+path;
    ofstream F;
    if (FILE *file = fopen(absolutePath.c_str(), "r")) {
        fclose(file);
    } else {
        ofstream F;
        F.open(absolutePath);
        F.close();
    }
    if(!format()){
        throw "文件格式错误";
    }
    int k=rand();
    sem=semget((key_t)k,1,0666|IPC_CREAT);
    set_semvalue(sem,1);
    write("hash",str);
}

NFile::NFile(uint256 hash, string kind,bool erase){
    string str;
    stringstream ss;
    ss<<hex<<hash;
    ss>>str;
    path = str+"."+kind;
    absolutePath=root_path+"/"+kind+"/"+path;
    ofstream F;
    F.open(absolutePath);
    F.close();
    if(!format()){
        throw "文件格式错误";
    }
    int k=rand();
    sem=semget((key_t)k,1,0666|IPC_CREAT);
    set_semvalue(sem,1);
    write("hash",str);
}
NFile::~NFile(){
    // multi process use one sem 
    del_semvalue(sem);
}
bool NFile::format(){
    //功能完善：检查文件格式
    return true;
}

bool NFile::write(string key, string value){
    remove(key);
    lock();
    ofstream file_writer;
    file_writer.open(absolutePath,ios::app);
    file_writer<<key<<endl;
    file_writer<<value<<endl;
    file_writer.flush();
    file_writer.close();
    release();
    return true;
}

bool NFile::write(string key,vector<string> value){
    remove(key);
    lock();
    ofstream file_writer;
    file_writer.open(absolutePath,ios::app);
    file_writer<<key<<" "<<value.size()<<endl;
    for(int i=0;i<value.size();i++){
        file_writer<<value[i]<<endl;
    }
    file_writer.flush();
    file_writer.close();
    release();
    return true;
}

string NFile::get(string key){
    lock();
    string ans = "";
    string line;
    ifstream in(absolutePath);
    int num = 0;
    string key_2;
    string num_str;
    while(getline(in,line)){
        //get one line data
        if(strcmp(key.c_str(),line.c_str())==0){
            getline(in,line);
            release();
            return line;
        }
        //get multi line data
        else if(key.size()<line.size()&&line[key.size()]==' '){
            key_2 = line.substr(0,key.size());
            if(strcmp(key.c_str(),key_2.c_str())==0){
                num_str=line.substr(key.size()+1,line.size()-key.size()-1);
                num=atoi(num_str.c_str());
                while(num--){
                    getline(in,line);
                    ans+=line+"\n";
                }
                release();
                return ans;
            }
        }
    }
    release();
    return ans;
}

void NFile::lock(){
    lock_p(sem);
}

void NFile::release(){
    lock_v(sem);
}



bool NFile::remove(string key){
    string g = get(key);
    if(g.size()==0){
        return false;
    }
    lock();
    ifstream in(absolutePath);
    string line;
    string data="";
    string key_2;
    int num;
    string num_str;
    while(getline(in,line)){
        if(strcmp(line.c_str(),key.c_str())==0){
            getline(in,line);
            continue;
        }
        else if(key.size()<line.size()&&line[key.size()]==' '){
            key_2 = line.substr(0,key.size());
            if(strcmp(key.c_str(),key_2.c_str())==0){
                num_str=line.substr(key.size()+1,line.size()-key.size()-1);
                num=atoi(num_str.c_str());
                while(num--){
                    getline(in,line);
                }
            }
        }
        else{
            data+=line+"\n";
        }
    }
    ofstream out;
    out.open(absolutePath);
    out<<data<<endl;
    out.flush();
    out.close();
    release();
    return true;
}

bool NFile::remove(string key,string val){
    string g = get(key);
    if(g.size()==0){
        return false;
    }
    lock();
    ifstream in(absolutePath);
    string line;
    string data="";
    string key_2;
    int num;
    string num_str;
    while(getline(in,line)){
        if(strcmp(line.c_str(),key.c_str())==0){
            getline(in,line);
            continue;
        }
        else if(key.size()<line.size()&&line[key.size()]==' '){
            key_2 = line.substr(0,key.size());
            if(strcmp(key.c_str(),key_2.c_str())==0){
                num_str=line.substr(key.size()+1,line.size()-key.size()-1);
                num=atoi(num_str.c_str());
                data+=key+" ";
                data+=to_string(num-1)+'\n';
                bool success=false;
                while(num--){
                    getline(in,line);
                    if(line==val){
                        success=true;
                        continue;
                    }
                    else
                    {  
                        data+=line+'\n';
                    }
                    
                }
                if(!success){
                    release();
                    return false;
                }
            }
        }
        else{
            data+=line+"\n";
        }
    }
    ofstream out;
    out.open(absolutePath);
    out<<data<<endl;
    out.flush();
    out.close();
    release();
    return true;
}
vector<string> NFile::gtv(string key){
    string ret =get(key);
    vector<string> ans;
    int beg=0;
    for(int i=0;i<ret.size();i++){
        if(ret[i]=='\n'){
            ans.push_back(ret.substr(beg,i-beg));
            beg=i;
        }
    }
    if(ans.size()==0){
        ans.push_back(ret);
    }
    return ans;
}
bool NFile::add(string key,vector<string> value){
    vector<string> ret = gtv(key);
    remove(key);
    for(int i=0;i<value.size();i++){
        ret.push_back(value[i]);
    }
    return write(key,ret);
}
bool NFile::add(string key,string value){
    vector<string> vv;
    vv.push_back(value);
    return add(key,vv);
}
