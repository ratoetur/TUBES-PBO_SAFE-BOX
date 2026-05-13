#ifndef SHIFTCIPHER_H
#define SHIFTCIPHER_H

#include "Cipher.h"
#include <string>
#include <vector>

using namespace std;

class ShiftCipher : public Cipher {
public:
    ShiftCipher(string key);

    vector<char> encrypt(const vector<char>& data) override;
    vector<char> decrypt(const vector<char>& data) override;
    string getAlgorithmName() const override;

    ~ShiftCipher();
};

#endif