# SafeBox File Encryption

SafeBox adalah program C++ sederhana untuk mengenkripsi dan mendekripsi file menggunakan konsep Object-Oriented Programming.

## Fitur

1. Algorithm Selector
   - User dapat memilih algoritma enkripsi.
   - Saat ini tersedia XOR Cipher.

2. File Encryption
   - User memasukkan file input, file output, dan key.
   - Program membaca file secara binary dan menghasilkan file terenkripsi.

3. File Decryption
   - User dapat mendekripsi file terenkripsi menggunakan key yang sama.

## Cara Compile

```bash
g++ src/*.cpp -Iinclude -o SafeBox

g++ src/main.cpp src/FileManager.cpp src/FileRegistry.cpp src/XorCipher.cpp src/CaesarCipher.cpp src/ShiftCipher.cpp -Iinclude -o safebox.exe
