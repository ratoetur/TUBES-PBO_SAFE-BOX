#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <vector>
#include <string>
using namespace std;

class FileManager {
public:
    vector<char> readBinaryFile(const string& filename);
    void writeBinaryFile(const string& filename, const vector<char>& data);
};

#endif