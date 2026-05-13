#include <iostream>
#include <vector>
#include <exception>

#include "../include/Cipher.h"
#include "../include/XorCipher.h"
#include "../include/CaesarCipher.h"
#include "../include/ShiftCipher.h"
#include "../include/FileManager.h"

using namespace std;

Cipher* pilihAlgoritma(const string& key) {
    int pilihanAlgoritma;

    cout << "Pilih algoritma enkripsi:" << endl;
    cout << "1. XOR Cipher" << endl;
    cout << "2. Caesar Cipher" << endl;
    cout << "3. Shift Cipher" << endl;
    cout << "Pilihan algoritma: ";
    cin >> pilihanAlgoritma;

    if (pilihanAlgoritma == 1) {
        return new XorCipher(key);
    }
    else if (pilihanAlgoritma == 2) {
        return new CaesarCipher(key);
    }
    else if (pilihanAlgoritma == 3) {
        return new ShiftCipher(key);
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
                vector<char> encrypted = cipher->encrypt(data);

                fileManager.writeBinaryFile(outputFile, encrypted);

                cout << "Algoritma: " << cipher->getAlgorithmName() << endl;
                cout << "File berhasil dienkripsi ke: " << outputFile << endl;

                delete cipher;
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
                vector<char> decrypted = cipher->decrypt(data);

                fileManager.writeBinaryFile(outputFile, decrypted);

                cout << "Algoritma: " << cipher->getAlgorithmName() << endl;
                cout << "File berhasil didekripsi ke: " << outputFile << endl;

                delete cipher;
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