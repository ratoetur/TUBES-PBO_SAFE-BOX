#ifndef CIPHER_H
#define CIPHER_H
#include <string>
#include <vector>
#include <fstream>
#include <cstdio>
#include <stdexcept>

using namespace std;

class SecureBuffer {
private:
    vector<unsigned char> buffer;

    void secureClear() {
        for (size_t i = 0; i < buffer.size(); i++) {
            buffer[i] = 0;
        }
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

    size_t size() const { return buffer.size(); }
    unsigned char* data() { return buffer.data(); }
    const unsigned char* data() const { return buffer.data(); }
    
    unsigned char& operator[](size_t i) { return buffer[i]; }
    const unsigned char& operator[](size_t i) const { return buffer[i]; }

    vector<unsigned char> toVector() const { return buffer; }

    void append(const SecureBuffer& other) {
        buffer.insert(buffer.end(), other.buffer.begin(), other.buffer.end());
    }

    SecureBuffer xorWith(const SecureBuffer& other) const {
        SecureBuffer result(buffer.size());
        for (size_t i = 0; i < buffer.size(); i++) {
            result[i] = buffer[i] ^ other.buffer[i % other.buffer.size()];
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

class FileManager {
private:
    string filePath;

    vector<char> readRawBytes() {
        ifstream file(filePath, ios::binary | ios::ate);
        if (!file.is_open()) throw runtime_error("Cannot open file");
        streamsize size = file.tellg();
        file.seekg(0, ios::beg);
        vector<char> buffer(size);
        if (!file.read(buffer.data(), size)) throw runtime_error("Error reading file");
        file.close();
        return buffer;
    }

    bool writeRawBytes(const vector<char>& data) {
        ofstream file(filePath, ios::binary);
        if (!file.is_open()) return false;
        file.write(data.data(), data.size());
        file.close();
        return file.good() || data.empty();
    }

public:
    FileManager() {}
    explicit FileManager(const string& path) : filePath(path) {}

    void setFilePath(const string& path) { filePath = path; }
    string getFilePath() const { return filePath; }

    bool exists() const {
        ifstream f(filePath);
        return f.good();
    }

    size_t getFileSize() const {
        ifstream file(filePath, ios::binary | ios::ate);
        return file.tellg();
    }

    bool removeFile() {
        return remove(filePath.c_str()) == 0;
    }

    string readAsString() {
        ifstream file(filePath);
        if (!file.is_open()) throw runtime_error("Cannot open file");
        string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();
        return content;
    }

    SecureBuffer readAsSecureBuffer() {
        vector<char> raw = readRawBytes();
        SecureBuffer sec(raw.size());
        for (size_t i = 0; i < raw.size(); i++) {
            sec[i] = static_cast<unsigned char>(raw[i]);
        }
        raw.assign(raw.size(), 0);
        return sec;
    }

    bool writeString(const string& content) {
        ofstream file(filePath);
        if (!file.is_open()) return false;
        file << content;
        file.close();
        return true;
    }

    bool writeSecureBufferToFile(const SecureBuffer& buffer) {
        vector<unsigned char> sec = buffer.toVector();
        vector<char> data(sec.begin(), sec.end());
        return writeRawBytes(data);
    }

    bool encryptFile(Cipher& cipher) {
        vector<char> raw = readRawBytes();
        vector<char> enc = cipher.encrypt(raw);
        raw.assign(raw.size(), 0);
        FileManager encFile(filePath + ".enc");
        return encFile.writeRawBytes(enc);
    }

    bool decryptFile(Cipher& cipher) {
        vector<char> enc = readRawBytes();
        vector<char> dec = cipher.decrypt(enc);
        enc.assign(enc.size(), 0);
        string decPath = filePath.length() > 4 && filePath.substr(filePath.length() - 4) == ".enc" 
                         ? filePath.substr(0, filePath.length() - 4) : filePath + ".dec";
        FileManager decFile(decPath);
        return decFile.writeRawBytes(dec);
    }

    SecureBuffer readAndDecrypt(Cipher& cipher) {
        SecureBuffer encBuf = readAsSecureBuffer();
        vector<unsigned char> sec = encBuf.toVector();
        vector<char> encData(sec.begin(), sec.end());
        vector<char> decData = cipher.decrypt(encData);
        encData.assign(encData.size(), 0);
        SecureBuffer res(decData.size());
        for (size_t i = 0; i < decData.size(); i++) {
            res[i] = static_cast<unsigned char>(decData[i]);
        }
        return res;
    }

    bool encryptAndWrite(Cipher& cipher, const SecureBuffer& plaintext) {
        vector<unsigned char> sec = plaintext.toVector();
        vector<char> raw(sec.begin(), sec.end());
        vector<char> enc = cipher.encrypt(raw);
        raw.assign(raw.size(), 0);
        return writeRawBytes(enc);
    }
};

#endif