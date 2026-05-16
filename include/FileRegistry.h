#ifndef FILEREGISTRY_H
#define FILEREGISTRY_H

#include <map>
#include <string>
#include <iostream>
using namespace std;

struct FileMeta {
    string algoritma;
    string passwordHint;
    string timestamp;
};

class FileRegistry {
    map<string, FileMeta> reg;
public:
    void tambah(const string& file, const string& algo,
                const string& hint, const string& waktu);

    void tampilkan() const;
};

#endif