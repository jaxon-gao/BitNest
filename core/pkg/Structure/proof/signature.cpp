#include<proof/signature.h>
ostream& operator<<(ostream &os, const signature & sig){
        os<<hex<<sig.pk[1];
        os<<" ";
        os<<hex<<sig.pk[0];
        os<<" ";
        os<<hex<<sig.sig[1];
        os<<" ";
        os<<hex<<sig.sig[0];
        return os;
    }
istream& operator>>(istream &is, signature & sig){
    string str;
    is>>str;
    uint256 tmp;
    tmp=uint256(str);
    sig.pk[1]=tmp;


    is>>str;
    tmp=uint256(str);
    sig.pk[0]=tmp;


    is>>str;
    tmp=uint256(str);
    sig.sig[1]=tmp;


    is>>str;
    tmp=uint256(str);
    sig.sig[0]=tmp;

    return is;
}

signature::signature(uint256 PK[2], uint256 SIG[2]){
    pk[0]=PK[0];
    pk[1]=PK[1];
    sig[0]=SIG[0];
    sig[1]=SIG[1];
}


signature::signature(){
    pk[0]=0;
    pk[1]=0;
    sig[1]=0;
    sig[0]=0;
}


bool operator==(const signature &sig1, const signature &sig2){
    return (sig1.pk[0]==sig2.pk[0])&&(sig1.pk[1]==sig2.pk[1])&&(sig1.sig[0]==sig2.sig[0])&&(sig1.sig[1]==sig2.sig[1]);
}

string signature::data(){
    stringstream ss;
    ss<<*this;
    string ans;
    string ans1;
    string ans2;
    string ans3;
    string ans4;
    ss>>ans1;
    ss>>ans2;
    ss>>ans3;
    ss>>ans4;
    ans+=ans1;
    ans+=",";
    ans+=ans2;
    ans+=",";
    ans+=ans3;
    ans+=",";
    ans+=ans4;
    return ans;
}
signature::signature(string data){
    stringstream ss;
    ss<<data;
    string num;
    string nums[4];
    ss>>num;
    int beg=0;
    int k=0;
    for(int i=0;i<num.size();i++){
        if(num[i]==','){
            nums[k++]=num.substr(beg,i-beg);
            beg=i+1;
        }
        if(i==num.size()-1){
            nums[k++]=num.substr(beg,i-beg+1);
            beg=i+1;
        }
    }
    uint256 *tmp;
    this->pk[1]=uint256(nums[0]);
    this->pk[0]=uint256(nums[1]);
    this->sig[1]=uint256(nums[2]);
    this->sig[0]=uint256(nums[3]);
}