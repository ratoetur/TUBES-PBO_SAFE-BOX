#include "CaesarCipher.h"
#include <algorithm>
#include <string>

static int getShiftFromKey(const string& key) {
    if (key.empty()) {
        return 3;
    }

    try {
        return stoi(key) % 26;
    }
    catch (...) {
        int total = 0;

        for (char c : key) {
            total += static_cast<unsigned char>(c);
        }

        return total % 26;
    }
}

CaesarCipher::CaesarCipher(string key) {
    this->secretKey = key;
}

vector<char> CaesarCipher::encrypt(const vector<char>& data) {
    vector<char> result;
    int shift = getShiftFromKey(secretKey);

    for (char c : data) {
        if (c >= 'A' && c <= 'Z') {
            result.push_back((c - 'A' + shift) % 26 + 'A');
        }
        else if (c >= 'a' && c <= 'z') {
            result.push_back((c - 'a' + shift) % 26 + 'a');
        }
        else {
            result.push_back(c);
        }
    }

    return result;
}

vector<char> CaesarCipher::decrypt(const vector<char>& data) {
    vector<char> result;
    int shift = getShiftFromKey(secretKey);

    for (char c : data) {
        if (c >= 'A' && c <= 'Z') {
            result.push_back((c - 'A' - shift + 26) % 26 + 'A');
        }
        else if (c >= 'a' && c <= 'z') {
            result.push_back((c - 'a' - shift + 26) % 26 + 'a');
        }
        else {
            result.push_back(c);
        }
    }

    return result;
}

string CaesarCipher::getAlgorithmName() const {
    return "Caesar Cipher";
}

CaesarCipher::~CaesarCipher() {
    fill(secretKey.begin(), secretKey.end(), '\0');
}