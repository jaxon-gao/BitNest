#include<UserOvershot/UserOvershot.h>
#include<sstream>
using namespace std;
string UserOvershot::data(){
    stringstream ss;
    string ans;
    string tmp;
    ss<<hex<<h;
    ss>>tmp;
    ans+=tmp+' ';


    ss<<hex<<cash;
    ss>>tmp;
    ans+=tmp+' ';


    ss<<hex<<frezon;
    ss>>tmp;
    ans+=tmp+' ';

    for(int i=0;i<contracts.size();i++){
        ans+=contracts[i]->data(' ')+":";
    }
    return ans;
}
string getKind(string data){
    int k=0;
    int beg=0;
    for(int i=0;i<data.size();i++){
        if(data[i]==' '){
            k++;
            if(k==2){
                return data.substr(beg,i-beg);
            }
            beg=i+1;
        }

    }
}
UserOvershot::UserOvershot(string data){
    int beg=0;
    int k=0;
    string tmp;
    string tmp1;
    for(int i=0;i<data.size();i++){
        if(data[i]==' '){
            if(k==0){
                tmp=data.substr(beg,i-beg);
                h=uint256(tmp);
                k++;
            }

            else if(k==1){
                tmp=data.substr(beg,i-beg);
                cash=uint256(tmp);
                k++;
            }
            else if(k==2){
                tmp=data.substr(beg,i-beg);
                frezon=uint256(tmp);
                k++;
            }
            else{
                tmp=data.substr(beg,i-beg);
                break;
            }
            beg=i+1;
        }
    }
    beg=0;
    for(int i=0;i<tmp.size();i++){
        if(tmp[i]==':'){
            tmp1=tmp.substr(beg,i-beg);
            if(getKind(tmp1)=="payment"){
                contracts.push_back(new payment(tmp1));
            }
            if(getKind(tmp1)=="storage"){
                contracts.push_back(new Storage(tmp1));
                
            }
            if(getKind(tmp1)=="backup"){
                contracts.push_back(new BackUp(tmp1));
            }
            beg=i+1;
        }
    }
}

UserOvershot::UserOvershot(){}