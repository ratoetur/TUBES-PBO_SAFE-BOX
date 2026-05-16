#include "../include/FileRegistry.h"

void FileRegistry::tambah(const string& file, const string& algo,
                          const string& hint, const string& waktu) {
    reg[file] = {algo, hint, waktu};
}

void FileRegistry::tampilkan() const {
    if (reg.empty()) { cout << "(kosong)\n"; return; }
    cout << "\n===== DAFTAR FILE =====\n";
    for (auto& [f, m] : reg) {
        cout << "File      : " << f << "\n"
             << "Algoritma : " << m.algoritma << "\n"
             << "Hint Key  : " << m.passwordHint << "\n"
             << "Waktu     : " << m.timestamp << "\n"
             << "-----------------------------\n";
    }
}