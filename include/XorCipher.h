#ifndef XORCIPHER_H
#define XORCIPHER_H

#include "Cipher.h"
#include <string>
#include <vector>
using namespace std;

class XorCipher : public Cipher {
private:
    string key;

public:
    XorCipher(string key);

    vector<char> encrypt(const vector<char>& data) override;
    vector<char> decrypt(const vector<char>& data) override;

    ~XorCipher();
};

#endif