#ifndef SECUREBUFFER_H
#define SECUREBUFFER_H

#include <vector>
#include <algorithm>

using namespace std;

template <typename T>
class SecureBuffer {
private:
    vector<T> buffer;

public:
    void setData(const vector<T>& data) {
        buffer = data;
    }

    vector<T> getData() const {
        return buffer;
    }

    vector<T> toCharVector() const {
        return buffer;
    }

    void fromCharVector(const vector<T>& data) {
        clearSecure();
        buffer = data;
    }

    void clearSecure() {
        fill(buffer.begin(), buffer.end(), 0);
        buffer.clear();
    }

    ~SecureBuffer() {
        clearSecure();
    }
};

#endif