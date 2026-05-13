#include <iostream>
#include <vector>
#include <exception>

#include "../include/Cipher.h"
#include "../include/XorCipher.h"
#include "../include/CaesarCipher.h"
#include "../include/ShiftCipher.h"
#include "../include/FileManager.h"
#include "../include/SecureBuffer.h"
#include "../include/SecureKey.h"
#include "../include/SafeBoxException.h"

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

string inputKeyDenganValidasi() {
    string key;
    cout << "Masukkan key (minimal 8 karakter): ";
    cin >> key;
 
    SecureKey sk;
    sk.setKey(key); 
    return sk.getRawKey();
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
            string inputFile, outputFile;

            cout << "Masukkan nama file input: ";
            cin >> inputFile;

            cout << "Masukkan nama file output: ";
            cin >> outputFile;

             try {
                string key = inputKeyDenganValidasi();
 
                FileManager fileManager;
 
                if (!fileManager.exists(inputFile)) {
                    throw FileNotFoundException(inputFile);
                }
 
                Cipher* cipher = pilihAlgoritma(key);
                if (cipher == nullptr) {
                    cout << "Algoritma tidak valid!" << endl;
                    continue;
                }
 
                vector<char> data = fileManager.readBinaryFile(inputFile);
 
                SecureBuffer<char> buffer;
                buffer.setData(data);
 
                fileManager.encryptFile(inputFile, *cipher);
 
                //cb
                if (fileManager.removeFile(inputFile)) {
                    cout << "File asli berhasil dihapus." << endl;
                } else {
                    cout << "Warning: file asli gagal dihapus." << endl;
                }
                //cb
                delete cipher;

                 cout << "File berhasil dienkripsi ke: " << outputFile << endl;

            }
            catch (const invalid_argument& e) {
                cout << "Key tidak valid: " << e.what() << endl;
            }
            catch (const SafeBoxException& e) {
                cout << e.what() << endl;
            }
            catch (const exception& e) {
                cout << "Error: " << e.what() << endl;
            }

        }
        else if (pilihan == 2) {
            string inputFile, outputFile;

            cout << "Masukkan nama file input terenkripsi: ";
            cin >> inputFile;

            cout << "Masukkan nama file output hasil dekripsi: ";
            cin >> outputFile;

            try {
                string key = inputKeyDenganValidasi();
 
                FileManager fileManager;
 
                if (!fileManager.exists(inputFile)) {
                    throw FileNotFoundException(inputFile);
                }
 
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
 
                //cb
                fileManager.decryptFile(inputFile, *cipher);

                if (fileManager.removeFile(inputFile)) {
                    cout << "File terenkripsi berhasil dihapus." << endl;
                } else {
                    cout << "Warning: file terenkripsi gagal dihapus." << endl;
                }
                //cb
                delete cipher;

                cout << "File berhasil didekripsi ke: " << outputFile << endl;

                delete cipher;
            }
            catch (const invalid_argument& e) {
                cout << "Key tidak valid: " << e.what() << endl;
            }
            catch (const SafeBoxException& e) {
                cout << e.what() << endl;
            }
            catch (const exception& e) {
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