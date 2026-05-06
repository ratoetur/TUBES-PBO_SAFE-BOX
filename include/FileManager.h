#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <vector>
#include <string>
#include "Cipher.h"

using namespace std;

class FileManager {
public:
    vector<char> readBinaryFile(const string& filename);
    void writeBinaryFile(const string& filename, const vector<char>& data);

    string readAsString(const string& filename);
    void writeString(const string& filename, const string& content);
    bool exists(const string& filename);
    bool removeFile(const string& filename);

    SecureBuffer readAsSecureBuffer(const string& filename);
    void writeSecureBuffer(const string& filename, const SecureBuffer& buffer);

    bool encryptFile(const string& filename, Cipher& cipher);
    bool decryptFile(const string& filename, Cipher& cipher);
    SecureBuffer readAndDecrypt(const string& filename, Cipher& cipher);
    void encryptAndWrite(const string& filename, Cipher& cipher, const SecureBuffer& buffer);
};

#endif