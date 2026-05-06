#ifndef CIPHER_H
#define CIPHER_H

#include <vector>
using namespace std;

class Cipher {
public:
    virtual vector<char> encrypt(const vector<char>& data) = 0;
    virtual vector<char> decrypt(const vector<char>& data) = 0;

    virtual ~Cipher() {}

    bool operator==(const string& inputKey) const {
        return secretKey == inputKey;
    }
};

#endif