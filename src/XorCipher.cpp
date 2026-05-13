#include "../include/XorCipher.h"
#include <algorithm>

XorCipher::XorCipher(const string& key) {
    this->secretKey = key;
}

vector<char> XorCipher::encrypt(const vector<char>& data) {
    vector<char> result;

    if (secretKey.empty()) {
        return data;
    }

    for (int i = 0; i < data.size(); i++) {
        char encryptedChar = data[i] ^ secretKey[i % secretKey.length()];
        result.push_back(encryptedChar);
    }

    return result;
}

vector<char> XorCipher::decrypt(const vector<char>& data) {
    return encrypt(data);
}

string XorCipher::getAlgorithmName() const {
    return "XOR Cipher";
}

SecureBuffer<char> XorCipher::encryptSecure(const SecureBuffer<char>& data) {
    vector<char> encrypted = encrypt(data.toCharVector());
    SecureBuffer<char> result;
    result.fromCharVector(encrypted);
    return result;
}
 
SecureBuffer<char> XorCipher::decryptSecure(const SecureBuffer<char>& data) {
    return encryptSecure(data);
}

XorCipher::~XorCipher() {
    fill(secretKey.begin(), secretKey.end(), '\0');
}