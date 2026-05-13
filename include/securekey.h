#ifndef SECUREKEY_H
#define SECUREKEY_H
#include <string>
#include <stdexcept>

class SecureKey {
private:
    std::string secretkey;
    std::string salt;

public:
    // Satu satunya cara set key: lewat setter ini
    void setKey(const std::string& key) {
        if (key.length() < 8) {
            throw std::invalid_argument("Key minimal 8 Karakter!!!");
        }
        secretkey = key;
    }

    void setSalt(const std::string& s) {
        if (s.length() < 4) {
            throw std::invalid_argument("Salt minimal 4 karakter!!!");
        }
        salt = s;
    }

    std::string getMaskedKey() const {
        return secretkey.substr(0, 2) + "******";
    }

    // Khusus dipakai internal oleh proses enkripsi
    std::string getRawKey() const { return secretkey; }
    std::string getRawSalt() const { return salt; }
};

#endif