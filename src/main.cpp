#include <iostream>
#include <vector>
#include <exception>

#include "Cipher.h"
#include "XorCipher.h"
#include "FileManager.h"
#include "SecureBuffer.h"

using namespace std;

Cipher* pilihAlgoritma(const string& key) {
    int pilihanAlgoritma;

    cout << "Pilih algoritma enkripsi:" << endl;
    cout << "1. XOR Cipher" << endl;
    cout << "Pilihan algoritma: ";
    cin >> pilihanAlgoritma;

    if (pilihanAlgoritma == 1) {
        return new XorCipher(key);
    }

    return nullptr;
}

int main() {
    int pilihan;

    do {
        cout << "====================================" << endl;
        cout << "       SafeBox File Encryption       " << endl;
        cout << "====================================" << endl;
        cout << "1. Encrypt File" << endl;
        cout << "2. Decrypt File" << endl;
        cout << "3. Exit" << endl;
        cout << "Pilih menu: ";
        cin >> pilihan;

        cout << endl;

        if (pilihan == 1) {
            string inputFile, outputFile, key;

            cout << "Masukkan nama file input: ";
            cin >> inputFile;

            cout << "Masukkan nama file output: ";
            cin >> outputFile;

            cout << "Masukkan key: ";
            cin >> key;

            try {
                FileManager fileManager;
                Cipher* cipher = pilihAlgoritma(key);

                if (cipher == nullptr) {
                    cout << "Algoritma tidak valid!" << endl;
                    continue;
                }

                vector<char> data = fileManager.readBinaryFile(inputFile);

                SecureBuffer<char> buffer;
                buffer.setData(data);

                vector<char> encrypted = cipher->encrypt(buffer.getData());
                fileManager.writeBinaryFile(outputFile, encrypted);

                delete cipher;

                cout << "File berhasil dienkripsi ke: " << outputFile << endl;
            }
            catch (exception& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
        else if (pilihan == 2) {
            string inputFile, outputFile, key;

            cout << "Masukkan nama file input terenkripsi: ";
            cin >> inputFile;

            cout << "Masukkan nama file output hasil dekripsi: ";
            cin >> outputFile;

            cout << "Masukkan key: ";
            cin >> key;

            try {
                FileManager fileManager;
                Cipher* cipher = pilihAlgoritma(key);

                if (cipher == nullptr) {
                    cout << "Algoritma tidak valid!" << endl;
                    continue;
                }

                vector<char> data = fileManager.readBinaryFile(inputFile);

                SecureBuffer<char> buffer;
                buffer.setData(data);

                vector<char> decrypted = cipher->decrypt(buffer.getData());
                fileManager.writeBinaryFile(outputFile, decrypted);

                delete cipher;

                cout << "File berhasil didekripsi ke: " << outputFile << endl;
            }
            catch (exception& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
        else if (pilihan == 3) {
            cout << "Keluar dari program..." << endl;
        }
        else {
            cout << "Pilihan tidak valid!" << endl;
        }

        cout << endl;

    } while (pilihan != 3);

    return 0;
}