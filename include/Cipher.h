#ifndef CIPHER_H
#define CIPHER_H

#include <string>
#include <vector>

using namespace std;

class Cipher {
protected:
    string secretKey;

public:
    virtual vector<char> encrypt(const vector<char>& data) = 0;
    virtual vector<char> decrypt(const vector<char>& data) = 0;
    virtual string getAlgorithmName() const = 0;

    virtual ~Cipher() {}

    bool operator==(const string& inputKey) const {
        return secretKey == inputKey;
    }
};

#endif