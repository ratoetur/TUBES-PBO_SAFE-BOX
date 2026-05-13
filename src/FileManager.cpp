#include "FileManager.h"
#include <fstream>
#include <stdexcept>
#include <cstdio>

using namespace std;

vector<char> FileManager::readBinaryFile(const string& filename) {
    ifstream file(filename, ios::binary);

    if (!file) {
        throw runtime_error("File input tidak bisa dibuka!");
    }

    vector<char> data(
        (istreambuf_iterator<char>(file)),
        istreambuf_iterator<char>()
    );

    file.close();
    return data;
}

void FileManager::writeBinaryFile(const string& filename, const vector<char>& data) {
    ofstream file(filename, ios::binary);

    if (!file) {
        throw runtime_error("File output tidak bisa dibuat!");
    }

    file.write(data.data(), data.size());
    file.close();
}

string FileManager::readAsString(const string& filename) {
    ifstream file(filename);

    if (!file) {
        throw runtime_error("File input tidak bisa dibuka!");
    }

    string content(
        (istreambuf_iterator<char>(file)),
        istreambuf_iterator<char>()
    );

    file.close();
    return content;
}

void FileManager::writeString(const string& filename, const string& content) {
    ofstream file(filename);

    if (!file) {
        throw runtime_error("File output tidak bisa dibuat!");
    }

    file << content;
    file.close();
}

bool FileManager::exists(const string& filename) {
    ifstream f(filename);
    return f.good();
}

bool FileManager::removeFile(const string& filename) {
    return remove(filename.c_str()) == 0;
}

SecureBuffer FileManager::readAsSecureBuffer(const string& filename) {
    vector<char> raw = readBinaryFile(filename);

    SecureBuffer sec;
    sec.fromCharVector(raw);

    raw.assign(raw.size(), 0);

    return sec;
}

void FileManager::writeSecureBuffer(const string& filename, const SecureBuffer& buffer) {
    writeBinaryFile(filename, buffer.toCharVector());
}

bool FileManager::encryptFile(const string& filename, Cipher& cipher) {
    vector<char> raw = readBinaryFile(filename);
    vector<char> enc = cipher.encrypt(raw);

    raw.assign(raw.size(), 0);

    writeBinaryFile(filename + ".enc", enc);

    return true;
}

bool FileManager::decryptFile(const string& filename, Cipher& cipher) {
    vector<char> enc = readBinaryFile(filename);
    vector<char> dec = cipher.decrypt(enc);

    enc.assign(enc.size(), 0);

    string decPath;

    if (filename.length() > 4 && filename.substr(filename.length() - 4) == ".enc") {
        decPath = filename.substr(0, filename.length() - 4);
    }
    else {
        decPath = filename + ".dec";
    }

    writeBinaryFile(decPath, dec);

    return true;
}

SecureBuffer FileManager::readAndDecrypt(const string& filename, Cipher& cipher) {
    SecureBuffer encBuf = readAsSecureBuffer(filename);

    vector<char> encData = encBuf.toCharVector();
    vector<char> decData = cipher.decrypt(encData);

    encData.assign(encData.size(), 0);

    SecureBuffer res;
    res.fromCharVector(decData);

    return res;
}

void FileManager::encryptAndWrite(const string& filename, Cipher& cipher, const SecureBuffer& buffer) {
    vector<char> raw = buffer.toCharVector();
    vector<char> enc = cipher.encrypt(raw);

    raw.assign(raw.size(), 0);

    writeBinaryFile(filename, enc);
}