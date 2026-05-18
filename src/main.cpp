#include <iostream>
#include <vector>
#include <exception>
#include <ctime>

#include "../include/Cipher.h"
#include "../include/XorCipher.h"
// #include "../include/CaesarCipher.h" // Comment dulu kalau belum ada .cpp-nya
// #include "../include/ShiftCipher.h"  // Comment dulu kalau belum ada .cpp-nya
#include "../include/FileManager.h"
#include "../include/SecureBuffer.h"
// #include "../include/SecureKey.h"
// #include "../include/SafeBoxException.h"
#include "../include/FileRegistry.h"

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
    // else if (pilihanAlgoritma == 2) {
    //     return new CaesarCipher(key);
    // }
    // else if (pilihanAlgoritma == 3) {
    //     return new ShiftCipher(key);
    // }

    cout << "Hanya bisa pilih 1 (XOR) untuk sekarang!" << endl;
    return new XorCipher(key); // Fallback aman
}

string inputKeyDenganValidasi() {
    string key;
    cout << "Masukkan key (minimal 8 karakter): ";
    cin >> key;
    
    // Sementara bypass SecureKey dulu supaya gak error, tapi tetap cek panjangnya
    while (key.length() < 8) {
        cout << "Key terlalu pendek! Minimal 8 karakter: ";
        cin >> key;
    }
    
    return key;
}

bool verifikasiKey(Cipher* cipher, const string& key) {
    return *cipher == key;  // operator== kepake di sini
}

string getCurrentTime() {
    time_t now = time(0);
    string dt = ctime(&now);
    dt.pop_back(); 
    return dt;
}

string buat_hint(const string& key) {
    if (key.length() <= 4) return "****";
    return key.substr(0, 2) + string(key.length() - 4, '*') + key.substr(key.length() - 2);
}

int main() {
    int pilihan;
    FileRegistry registry;

    do {
        cout << "====================================" << endl;
        cout << "       SafeBox File Encryption       " << endl;
        cout << "====================================" << endl;
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

            // --- OTOMATIS JADIN .sbox ---
            size_t titikTerakhir = inputFile.find_last_of(".");
            
            if (titikTerakhir != string::npos) {
                outputFile = inputFile.substr(0, titikTerakhir) + ".sbox";
            } else {
                outputFile = inputFile + ".sbox";
            }
            // -----------------------------

            try {
                string key = inputKeyDenganValidasi();

                string keyKonfirmasi;
                cout << "Konfirmasi key: ";
                cin >> keyKonfirmasi;

                Cipher* cekKunci = new XorCipher(key);
                if (!verifikasiKey(cekKunci, keyKonfirmasi)) {
                    cout << "Key tidak cocok! Proses enkripsi dibatalkan." << endl;
                    delete cekKunci;
                    continue;
                }
                delete cekKunci;

                FileManager fileManager;
 
                if (!fileManager.exists(inputFile)) {
                    throw runtime_error("File input tidak ditemukan!");
                }
 
                Cipher* cipher = pilihAlgoritma(key);
 
                vector<char> data = fileManager.readBinaryFile(inputFile);
                vector<char> encrypted = cipher->encrypt(data);
 
                fileManager.writeBinaryFile(outputFile, encrypted);
 
                if (fileManager.removeFile(inputFile)) {
                    cout << "File asli berhasil dihapus." << endl;
                } else {
                    cout << "Warning: file asli gagal dihapus." << endl;
                }

                delete cipher;
                cout << "File berhasil dienkripsi ke: " << outputFile << endl;

                registry.tambah(
                    outputFile,
                    "XOR",
                    buat_hint(key),
                    getCurrentTime()
                );
            }
            catch (const exception& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
               else if (pilihan == 2) {
            string inputFile, outputFile;

            cout << "Masukkan nama file input terenkripsi: ";
            cin >> inputFile;

            // --- OTOMATIS BALIKIN JADI .txt ---
            size_t posSbox = inputFile.find(".sbox");
            
            if (posSbox != string::npos) {
                // Kalau ketemu .sbox (misal: pesan.sbox), potong dan ganti jadi .txt
                outputFile = inputFile.substr(0, posSbox) + ".txt";
            } else {
                // Kalau lupa ketik .sbox (misal: pesan), langsung tambahin .txt
                outputFile = inputFile + ".txt";
            }
            // -----------------------------------

            try {
                string key = inputKeyDenganValidasi();
                FileManager fileManager;
 
                if (!fileManager.exists(inputFile)) {
                    throw runtime_error("File input tidak ditemukan!");
                }
 
                Cipher* cipher = pilihAlgoritma(key);
 
                vector<char> data = fileManager.readBinaryFile(inputFile);
                vector<char> decrypted = cipher->decrypt(data);

                fileManager.writeBinaryFile(outputFile, decrypted);
 
                if (fileManager.removeFile(inputFile)) {
                    cout << "File terenkripsi berhasil dihapus." << endl;
                } else {
                    cout << "Warning: file terenkripsi gagal dihapus." << endl;
                }

                delete cipher;
                cout << "File berhasil didekripsi ke: " << outputFile << endl;

                registry.tambah(
                    outputFile,
                    "DECRYPT-XOR",
                    buat_hint(key),
                    getCurrentTime()
                );
            }
            catch (const exception& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
        else if (pilihan == 3) {
            registry.tampilkan();
        }
        else if (pilihan == 4) {
            cout << "Keluar dari program..." << endl;
        }
        else {
            cout << "Pilihan tidak valid!" << endl;
        }

        cout << endl;

    } while (pilihan != 4);

    return 0;

}