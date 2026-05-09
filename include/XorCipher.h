```cpp
#ifndef XORCIPHER_H
#define XORCIPHER_H

#include "Cipher.h"
#include "securebuffer.h"
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
    string getAlgorithmName() const override;

    SecureBuffer encryptSecure(const SecureBuffer& data);
    SecureBuffer decryptSecure(const SecureBuffer& data);

    ~XorCipher();
};

#endif
```