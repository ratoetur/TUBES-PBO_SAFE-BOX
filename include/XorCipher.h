#ifndef XORCIPHER_H
#define XORCIPHER_H

#include "../include/Cipher.h"
#include "../include/SecureBuffer.h"
#include <string>
#include <vector>

using namespace std;

class XorCipher : public Cipher {
public:
    XorCipher(const string& key);

    vector<char> encrypt(const vector<char>& data) override;
    vector<char> decrypt(const vector<char>& data) override;
    string getAlgorithmName() const override;

    SecureBuffer<char> encryptSecure(const SecureBuffer<char>& data);
    SecureBuffer<char> decryptSecure(const SecureBuffer<char>& data);

    ~XorCipher();
};

#endif
