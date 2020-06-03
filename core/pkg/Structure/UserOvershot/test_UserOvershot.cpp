#include <UserOvershot/test_UserOvershot.h>
bool test_userovershot(){    
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
    UserOvershot n();
    return true;
}