#ifndef SECUREBUFFER_H
#define SECUREBUFFER_H

#include <vector>
#include <algorithm>

using std::vector;
using std::fill;

template <typename T>
class SecureBuffer {
private:
    vector<T> buffer;

public:
    void push(const T& val) { 
        buffer.push_back(val); 
    }

    void setData(const vector<T>& data) {
        buffer = data;
    }

    vector<T> getData() const {
        return buffer;
    }

    int size() const {
        return buffer.size();
    }

    ~SecureBuffer() {
        fill(buffer.begin(), buffer.end(), 0);
    }
};

#endif