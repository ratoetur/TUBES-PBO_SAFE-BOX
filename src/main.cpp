#include <iostream>
#include <vector>
#include <exception>
#include <ctime>

#include "../include/Cipher.h"
#include "../include/XorCipher.h"
#include "../include/CaesarCipher.h"
#include "../include/ShiftCipher.h" 
#include "../include/FileManager.h"
#include "../include/SecureBuffer.h"
#include "../include/SecureKey.h"
#include "../include/SafeBoxException.h"
#include "../include/FileRegistry.h"

using namespace std;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void waitKey() {
    cout << "\nTekan Enter untuk melanjutkan...";
    cin.ignore();
    cin.get();
}

Cipher* pilihAlgoritma(const string& key) {
    int pilihanAlgoritma;

    while (true) {
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
        else {
            cout << "Pilihan tidak valid!" << endl;
        }
    }
}

string inputKeyDenganValidasi() {
    string key;
    cout << "Masukkan key (minimal 8 karakter): ";
    cin >> key;
    
    while (key.length() < 8) {
        cout << "Key terlalu pendek! Minimal 8 karakter: ";
        cin >> key;
    }
    
    return key;
}

bool verifikasiKey(Cipher* cipher, const string& key) {
    return *cipher == key;  
}

string getCurrentTime() {
    time_t now = time(0);
    string dt = ctime(&now);
    if (!dt.empty()) dt.pop_back();
    return dt;
}

string buat_hint(const string& key) {
    if (key.length() <= 4) return "****";
    return key.substr(0, 2) + string(key.length() - 4, '*') + key.substr(key.length() - 2);
}

int main() {
    int pilihan;
    FileRegistry registry;

    clearScreen();
    string username;

    cout << "====================================" << endl;
    cout << "       SafeBox File Encryption       " << endl;
    cout << "====================================" << endl;
    cout << " Username: "; cin >> username;
    waitKey();

    do {
        clearScreen();
        cout << "====================================" << endl;
        cout << "       SafeBox File Encryption       " << endl;
        cout << "====================================" << endl;
        cout << " User: " << username << endl;
        cout << "1. Encrypt File" << endl;
        cout << "2. Decrypt File" << endl;
        cout << "3. Riwayat File" << endl;
        cout << "4. Exit" << endl;
        cout << "Pilih menu: ";
        cin >> pilihan;

        cout << endl;

        if (pilihan == 1) {
            string inputFile, outputFile;
        
            cout << "Masukkan nama file input: ";
            cin >> inputFile;

            try {
                string key = inputKeyDenganValidasi();

                Cipher* cipher = pilihAlgoritma(key);
                string keyKonfirmasi;
                cout << "Konfirmasi key: ";
                cin >> keyKonfirmasi;

                if (!verifikasiKey(cipher, keyKonfirmasi)) {
                    cout << "Key tidak cocok! Proses dibatalkan." << endl;
                    delete cipher;
                    waitKey();
                    continue;
                }

                FileManager fileManager(username);
 
                if (!fileManager.exists(inputFile)) {
                    throw runtime_error("File input tidak ditemukan!");
                }
 
 
                fileManager.encryptFile(inputFile, *cipher);
 
                if (fileManager.removeFile(inputFile)) {
                    cout << "File asli berhasil dihapus." << endl;
                } else {
                    cout << "Warning: file asli gagal dihapus." << endl;
                }

                size_t titikTerakhir = inputFile.find_last_of(".");
                
                if (titikTerakhir != string::npos) {
                    outputFile = inputFile.substr(0, titikTerakhir) + ".sbox";
                } else {
                    outputFile = inputFile + ".sbox";
                }

                cout << "File berhasil dienkripsi ke: " << outputFile << endl;

                registry.tambah(
                    outputFile,
                    cipher->getAlgorithmName(),
                    buat_hint(key),
                    getCurrentTime()
                );
                delete cipher;
            }
            catch (const exception& e) {
                cout << "Error: " << e.what() << endl;
            }
            waitKey();
        }
        else if (pilihan == 2) {
            string inputFile, outputFile;
            
            cout << "Masukkan nama file input terenkripsi: ";
            cin >> inputFile;

            try {
                string key = inputKeyDenganValidasi();

                Cipher* cipher = pilihAlgoritma(key);
                FileManager fileManager(username);
 
                if (!fileManager.exists(inputFile)) {
                    throw runtime_error("File input tidak ditemukan!");
                }
 
                fileManager.decryptFile(inputFile, *cipher);
 
                if (fileManager.removeFile(inputFile)) {
                    cout << "File terenkripsi berhasil dihapus." << endl;
                } else {
                    cout << "Warning: file terenkripsi gagal dihapus." << endl;
                }

                size_t posSbox = inputFile.find(".sbox");
                
                if (posSbox != string::npos) {
                    outputFile = inputFile.substr(0, posSbox) + ".txt";
                } else {
                    outputFile = inputFile + ".txt";
                }

                cout << "File berhasil didekripsi ke: " << outputFile << endl;

                registry.tambah(
                    outputFile,
                    cipher->getAlgorithmName(),
                    buat_hint(key),
                    getCurrentTime()
                );
                delete cipher;
            }
            catch (const exception& e) {
                cout << "Error: " << e.what() << endl;
            }
            waitKey();
        }
        else if (pilihan == 3) {
            clearScreen();
            registry.tampilkan();
            waitKey();
        }
        else if (pilihan == 4) {
            cout << "Keluar dari program..." << endl;
        }
        else {
            cout << "Pilihan tidak valid!" << endl;
            waitKey();
        }

        cout << endl;

    } while (pilihan != 4);

    return 0;

}