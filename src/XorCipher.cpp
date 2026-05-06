#include "XorCipher.h"
#include <algorithm>

XorCipher::XorCipher(string key) {
    this->key = key;
}

vector<char> XorCipher::encrypt(const vector<char>& data) {
    vector<char> result;

    if (key.empty()) {
        return data;
    }

    for (int i = 0; i < data.size(); i++) {
        char encryptedChar = data[i] ^ key[i % key.length()];
        result.push_back(encryptedChar);
    }

    return result;
}

vector<char> XorCipher::decrypt(const vector<char>& data) {
    return encrypt(data);
}

XorCipher::~XorCipher() {
    fill(key.begin(), key.end(), '\0');
}