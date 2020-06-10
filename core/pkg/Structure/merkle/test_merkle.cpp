#include <merkle/test_merkel.h>
bool test_merkle()
{
    vector<uint8_t> nn;
    nn.push_back('H');
    nn.push_back('e');
    nn.push_back('l');
    nn.push_back('l');
    nn.push_back('o');
    nn.push_back(' ');
    nn.push_back('W');
    nn.push_back('o');
    nn.push_back('r');
    nn.push_back('l');
    nn.push_back('d');
    stringstream ss;
    string str;
    ss << hex << getHash(nn) << endl;
    ss >> str;
    if (str != "a591a6d40bf420404a011733cfb7b190d62c65bf0bcda32b57b277d9ad9f146e")
    {
        return false;
    }
    cout << hex << getHash("/home/jiang/Desktop/test/test_merkle") << endl;
    cout << "test: merkle test finished" << endl;
    return true;
}