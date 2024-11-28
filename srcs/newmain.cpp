#include <iostream>
#include <fstream>
#include <string>
#include <NTL/ZZ.h>
#include "RSA.hpp"
using namespace std;
using namespace NTL;


void saveKeyToFile(const string& filename, const ZZ& n, const ZZ& key) {
    ofstream keyFile(filename);
    if (!keyFile) {
        cerr << "Error: Unable to open file " << filename << " for writing." << endl;
        return;
    }
    keyFile << n << endl;
    keyFile << key << endl;
    keyFile.close();
}

pair<ZZ, ZZ> loadKeyFromFile(const string& filename) {
    ifstream keyFile(filename);
    if (!keyFile) {
        cerr << "Error: Unable to open file " << filename << " for reading." << endl;
        exit(1);
    }
    ZZ n, key;
    keyFile >> n >> key;
    keyFile.close();
    return {n, key};
}

void encryptFile() {
    string message;
    cout << "Enter message to encrypt: ";
    getline(cin, message);

    // Option to generate or import public key
    char choice;
    cout << "Generate new key pair? (y/n): ";
    cin >> choice;
    cin.ignore(); // Clear newline

    ZZ n, e, d;
    if (choice == 'y' || choice == 'Y') {
        // Generate new key pair
        generate_keypair(n, e, d);
        
        // Save public and private keys
        saveKeyToFile("public.key", n, e);
        saveKeyToFile("private.key", n, d);
        
        cout << "Keys generated and saved to public.key and private.key" << endl;
    } else {
        // Import public key
        auto publicKey = loadKeyFromFile("public.key");
        n = publicKey.first;
        e = publicKey.second;
    }

    // Convert message to number and encrypt
    ZZ m = stringToNumber(message);
    ZZ c = encrypt(m, e, n);

    // Save encrypted message to file
    ofstream cipherFile("cipher.txt");
    if (!cipherFile) {
        cerr << "Error: Unable to open cipher.txt for writing." << endl;
        return;
    }
    cipherFile << c;
    cipherFile.close();

    cout << "Message encrypted and saved to cipher.txt" << endl;
    cout << "Encrypted Message: " << c << endl;
}

void decryptFile() {
    // Import private key
    auto privateKey = loadKeyFromFile("private.key");
    ZZ n = privateKey.first;
    ZZ d = privateKey.second;

    // Read cipher from file
    ifstream cipherFile("cipher.txt");
    if (!cipherFile) {
        cerr << "Error: Unable to open cipher.txt for reading." << endl;
        return;
    }
    ZZ c;
    cipherFile >> c;
    cipherFile.close();

    // Decrypt
    ZZ decrypted_m = decrypt(c, d, n);
    string dmessage = numberToString(decrypted_m);

    cout << "Decrypted Message: " << decrypted_m << endl;
    cout << "Converted Message: " << dmessage << endl;
}

int main() {
    int choice;
    while (true) {
        cout << "\nRSA File Encryption/Decryption Menu:" << endl;
        cout << "1. Encrypt Message" << endl;
        cout << "2. Decrypt Message" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // Clear newline

        switch (choice) {
            case 1:
                encryptFile();
                break;
            case 2:
                decryptFile();
                break;
            case 3:
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
    return 0;
}

// Previous implementations of utility functions remain the same as in the original code
// (stringToNumber, numberToString, InverseMod, power_mod, MillerRabinTest, 
// random_prime, gcd, generate_keypair, encrypt, decrypt)