#include "../include/FileManager.h"
#include <fstream>
#include <cstdio>
#include <ctime>

using namespace std;

static string waktu_sekarang() {
    time_t now = time(0);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return string(buf);
}
 
void tulis_log(const string& aksi, const string& file,
               const string& user, const string& status) {
    ofstream log("audit.txt", ios::app);
    if (log.is_open())
        log << "[" << waktu_sekarang() << "] "
            << aksi
            << " | File: "   << file
            << " | User: "   << user
            << " | Status: " << status << "\n";
}

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
        throw SafeBoxException("File output tidak bisa dibuat: " + filename);
    }

    file.write(data.data(), data.size());
    file.close();
}

string FileManager::readAsString(const string& filename) {
    ifstream file(filename);

    if (!file) {
        throw SafeBoxException("File input tidak bisa dibuka: " + filename);
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
    throw SafeBoxException("File output tidak bisa dibuat: " + filename);    }

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

SecureBuffer<char> FileManager::readAsSecureBuffer(const string& filename) {
    vector<char> raw = readBinaryFile(filename);
    SecureBuffer<char> sec;

    sec.fromCharVector(raw);

    raw.assign(raw.size(), 0);

    return sec;
}

void FileManager::writeSecureBuffer(const string& filename, const SecureBuffer<char>& buffer) {
    writeBinaryFile(filename, buffer.toCharVector());
}

bool FileManager::encryptFile(const string& filename, Cipher& cipher) {
    try{
    SecureBuffer<char> plainBuffer = readAsSecureBuffer(filename);
    vector<char> plainData = plainBuffer.toCharVector();
    
    string header = "SAFE";
    vector<char> combinedData(header.begin(), header.end());
    combinedData.insert(combinedData.end(), plainData.begin(), plainData.end());

    vector<char> encryptedData = cipher.encrypt(combinedData);
    
    string outFile;

    size_t dot = filename.find_last_of('.');

    if (dot != string::npos) {
        outFile = filename.substr(0, dot) + ".sbox";
    } else {
        outFile = filename + ".sbox";
    }

    writeBinaryFile(outFile, encryptedData);
    
    plainBuffer.clearSecure();
        combinedData.assign(combinedData.size(), 0);
        
        tulis_log("ENCRYPT", filename, currentUser, "SUCCESS"); 
        return true;
    } 
    catch (const exception& e) {
        tulis_log("ENCRYPT", filename, currentUser, "FAILED: " + string(e.what()));
        throw;
    }
}


bool FileManager::decryptFile(const string& filename, Cipher& cipher) {
    try{
    SecureBuffer<char> encBuffer = readAsSecureBuffer(filename);
    vector<char> enc = encBuffer.toCharVector();

    vector<char> dec = cipher.decrypt(enc);

    encBuffer.clearSecure();

    if (dec.size() < 4) {
        throw InvalidKeyException();
    }

    string header(dec.begin(), dec.begin() + 4);

    if (header != "SAFE") {
        throw InvalidKeyException();
    }

    dec.erase(dec.begin(), dec.begin() + 4);

    string decPath =
    (filename.length() > 5 &&
     filename.substr(filename.length() - 5) == ".sbox")
    ? filename.substr(0, filename.length() - 5) + ".txt"
    : filename + ".dec";

    writeBinaryFile(decPath, dec);

    SecureBuffer<char> temp;
    temp.fromCharVector(dec);
    temp.clearSecure();
     
    tulis_log("DECRYPT", filename, currentUser, "SUCCESS");
    return true;
    } 
    catch (const InvalidKeyException&) {
        tulis_log("DECRYPT", filename, currentUser, "FAILED: Invalid Key");
        throw;
    } 
    catch (const exception& e) {
        tulis_log("DECRYPT", filename, currentUser, "FAILED: " + string(e.what()));
        throw;
    }
}


SecureBuffer<char> FileManager::readAndDecrypt(const string& filename, Cipher& cipher) {
    SecureBuffer<char> encBuf = readAsSecureBuffer(filename);

    vector<char> encData = encBuf.toCharVector();
    vector<char> decData = cipher.decrypt(encData);

    encData.assign(encData.size(), 0);
    
    SecureBuffer<char> res;

    res.fromCharVector(decData);

    return res;
}

void FileManager::encryptAndWrite(const string& filename, Cipher& cipher, const SecureBuffer<char>& buffer) {
    SecureBuffer<char> temp = buffer;
    vector<char> enc = cipher.encrypt(temp.toCharVector());

    temp.clearSecure();

    writeBinaryFile(filename, enc);

}
