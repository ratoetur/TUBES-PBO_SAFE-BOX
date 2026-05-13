#ifndef CAESARCIPHER_H
#define CAESARCIPHER_H

#include "Cipher.h"
#include <string>
#include <vector>

using namespace std;

class CaesarCipher : public Cipher {
public:
    CaesarCipher(string key);

    vector<char> encrypt(const vector<char>& data) override;
    vector<char> decrypt(const vector<char>& data) override;
    string getAlgorithmName() const override;

    ~CaesarCipher();
};

#endif