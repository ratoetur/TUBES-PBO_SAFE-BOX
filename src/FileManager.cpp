#include "FileManager.h"
#include <fstream>
#include <stdexcept>
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