#include "ShiftCipher.h"
#include <algorithm>
#include <string>

static int getByteShiftFromKey(const string& key) {
    if (key.empty()) {
        return 1;
    }

    try {
        return stoi(key) % 256;
    }
    catch (...) {
        int total = 0;

        for (char c : key) {
            total += static_cast<unsigned char>(c);
        }

        return total % 256;
    }
}

ShiftCipher::ShiftCipher(string key) {
    this->secretKey = key;
}

vector<char> ShiftCipher::encrypt(const vector<char>& data) {
    vector<char> result;
    int shift = getByteShiftFromKey(secretKey);

    for (char c : data) {
        unsigned char byte = static_cast<unsigned char>(c);
        unsigned char encrypted = static_cast<unsigned char>((byte + shift) % 256);
        result.push_back(static_cast<char>(encrypted));
    }

    return result;
}

vector<char> ShiftCipher::decrypt(const vector<char>& data) {
    vector<char> result;
    int shift = getByteShiftFromKey(secretKey);

    for (char c : data) {
        unsigned char byte = static_cast<unsigned char>(c);
        unsigned char decrypted = static_cast<unsigned char>((byte - shift + 256) % 256);
        result.push_back(static_cast<char>(decrypted));
    }

    return result;
}

string ShiftCipher::getAlgorithmName() const {
    return "Shift Cipher";
}

ShiftCipher::~ShiftCipher() {
    fill(secretKey.begin(), secretKey.end(), '\0');
}