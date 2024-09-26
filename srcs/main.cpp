#include "main.h"


// Function to compute the greatest common divisor (GCD)
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to compute the modular inverse of e mod phi(n)
int modInverse(int e, int phi) {
    int d = 0, x1 = 0, x2 = 1, y1 = 1;
    int tempPhi = phi;

    while (e > 0) {
        int temp1 = tempPhi / e;
        int temp2 = tempPhi - temp1 * e;
        tempPhi = e;
        e = temp2;

        int x = x2 - temp1 * x1;
        int y = d - temp1 * y1;

        x2 = x1;
        x1 = x;
        d = y1;
        y1 = y;
    }

    if (tempPhi == 1) {
        return d + phi;
    }

    return -1; // If modular inverse doesn't exist
}

// Function to generate public and private keys
pair<pair<int, int>, pair<int, int>> generateKeypair(int p, int q) {
    int n = p * q;
    int phi = (p - 1) * (q - 1);

    // Choose an integer e such that 1 < e < phi(n) and gcd(e, phi(n)) = 1
    int e = 7; // As per the example
    while (gcd(e, phi) != 1) {
        e++;
    }

    // Compute d, the modular inverse of e
    int d = modInverse(e, phi);

    // Public and private keys
    return make_pair(make_pair(e, n), make_pair(d, n));
}

// Function for modular exponentiation: (base^exp) % mod
int modExp(int base, int exp, int mod) {
    int result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {  // If exp is odd, multiply base with result
            result = (result * base) % mod;
        }
        base = (base * base) % mod;  // Square the base
        exp /= 2;  // Divide exp by 2
    }
    return result;
}

// Function to encrypt the message
int encrypt(int publicKey, int n, int plaintext) {
    return modExp(plaintext, publicKey, n);
}

// Function to decrypt the message
int decrypt(int privateKey, int n, int ciphertext) {
    return modExp(ciphertext, privateKey, n);
}

int main() {
    // Example primes (small ones for simplicity)
    int p = 11;
    int q = 7;

    // Generate public and private keys
    pair<pair<int, int>, pair<int, int>> keys = generateKeypair(p, q);
    pair<int, int> publicKey = keys.first;
    pair<int, int> privateKey = keys.second;

    // Display the keys
    cout << "Public Key: (" << publicKey.first << ", " << publicKey.second << ")\n";
    cout << "Private Key: (" << privateKey.first << ", " << privateKey.second << ")\n";

    // Message to encrypt (m = 9 as per the example)
    int message = 9;
    cout << "Original message: " << message << endl;

    // Encrypt the message
    int encryptedMessage = encrypt(publicKey.first, publicKey.second, message);
    cout << "Encrypted message: " << encryptedMessage << endl;

    // Decrypt the message
    int decryptedMessage = decrypt(privateKey.first, privateKey.second, encryptedMessage);
    cout << "Decrypted message: " << decryptedMessage << endl;

    return 0;
}
