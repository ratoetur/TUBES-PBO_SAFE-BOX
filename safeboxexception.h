#ifndef SAFEBOXEXCEPTION_H
#define SAFEBOXEXCEPTION_H
#include <stdexcept>
#include <string>

class SafeBoxException : public std::runtime_error {
public:
    explicit SafeBoxException(const std::string& pesan)
        : std::runtime_error("[SafeBox Error] " + pesan) {}
};

class FileNotFoundException : public SafeBoxException {
public:
    explicit FileNotFoundException(const std::string& namaFile)
        : SafeBoxException("File tidak ditemukan: " + namaFile) {}
};

class InvalidKeyException : public SafeBoxException {
public:
    explicit InvalidKeyException()
        : SafeBoxException("Kunci enkripsi sala atau tidak cocok!!!") {}
};

#endif