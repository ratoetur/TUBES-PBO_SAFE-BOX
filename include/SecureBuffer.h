#ifndef SECUREBUFFER_H
#define SECUREBUFFER_H

#include <vector>
#include <algorithm>
#include <string>
#include <cstdio>

using std::vector;
using std::string;

class SecureBuffer {
private:
    vector<unsigned char> buffer;

    void secureClear() {
        std::fill(buffer.begin(), buffer.end(), 0);
        buffer.clear();
    }

    SecureBuffer(const SecureBuffer&) = delete;
    SecureBuffer& operator=(const SecureBuffer&) = delete;

public:
    SecureBuffer() {}
    explicit SecureBuffer(size_t size) : buffer(size, 0) {}
    SecureBuffer(const unsigned char* data, size_t size) : buffer(data, data + size) {}

    SecureBuffer(SecureBuffer&& other) noexcept : buffer(std::move(other.buffer)) {}
    SecureBuffer& operator=(SecureBuffer&& other) noexcept {
        if (this != &other) {
            secureClear();
            buffer = std::move(other.buffer);
        }
        return *this;
    }

    ~SecureBuffer() {
        secureClear();
    }

    void push(unsigned char val) {
        buffer.push_back(val);
    }

    void setData(const vector<unsigned char>& data) {
        buffer = data;
    }

    vector<unsigned char> getData() const {
        return buffer;
    }

    size_t size() const {
        return buffer.size();
    }

    unsigned char& operator[](size_t i) { return buffer[i]; }
    const unsigned char& operator[](size_t i) const { return buffer[i]; }
    unsigned char* data() { return buffer.data(); }
    const unsigned char* data() const { return buffer.data(); }

    vector<char> toCharVector() const {
        return vector<char>(buffer.begin(), buffer.end());
    }

    void fromCharVector(const vector<char>& data) {
        secureClear();
        buffer.assign(data.begin(), data.end());
    }

    SecureBuffer xorWith(const SecureBuffer& key) const {
        SecureBuffer result(buffer.size());
        for (size_t i = 0; i < buffer.size(); i++) {
            result[i] = buffer[i] ^ key[i % key.size()];
        }
        return result;
    }

    string toHexString() const {
        string hex;
        char buf[3];
        for (size_t i = 0; i < buffer.size(); i++) {
            sprintf(buf, "%02X", buffer[i]);
            hex += buf;
        }
        return hex;
    }

    static SecureBuffer fromHexString(const string& hex) {
        SecureBuffer res(hex.length() / 2);
        for (size_t i = 0; i < res.size(); i++) {
            unsigned int byte;
            sscanf(hex.c_str() + i * 2, "%02x", &byte);
            res[i] = static_cast<unsigned char>(byte);
        }
        return res;
    }
};

#endif