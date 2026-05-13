#include "XorCipher.h"
#include <algorithm>

XorCipher::XorCipher(string key) {
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

SecureBuffer XorCipher::encryptSecure(const SecureBuffer& data) {
    vector<char> raw = data.toCharVector();
    vector<char> encrypted = encrypt(raw);

    SecureBuffer result;
    result.fromCharVector(encrypted);

    return result;
}

SecureBuffer XorCipher::decryptSecure(const SecureBuffer& data) {
    vector<char> raw = data.toCharVector();
    vector<char> decrypted = decrypt(raw);

    SecureBuffer result;
    result.fromCharVector(decrypted);

    return result;
}

XorCipher::~XorCipher() {
    fill(secretKey.begin(), secretKey.end(), '\0');
}