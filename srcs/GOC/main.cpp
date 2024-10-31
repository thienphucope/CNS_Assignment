#include <iostream>
#include <NTL/ZZ.h>
#include <gmp.h>
#include <ctime>

#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <random>




using namespace std;
using namespace NTL;

// Hàm kiểm tra số nguyên tố
bool is_prime(const ZZ& n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n%2 == 0) return false;
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

// Sinh số nguyên tố ngẫu nhiên
/*ZZ random_prime(int bit_length) {
    ZZ p;
    while (true) {
        p = RandomBits_ZZ(bit_length);
	if (!IsOdd(p)) p++;
        if (is_prime(p)) return p;
    }
}
*/
ZZ random_prime(int bit_length) {
	int num_threads = 100;
    ZZ found_prime; // Variable to store the found prime
    bool prime_found = false; // Flag to indicate if a prime has been found
    mutex mtx; // Mutex to protect access to found_prime

    auto worker = [&]() {
        while (!prime_found) {
            ZZ p = RandomBits_ZZ(bit_length); // Generate random number
            if (is_prime(p)) {
                lock_guard<mutex> lock(mtx); // Lock the mutex
                if (!prime_found) { // Double-check to avoid race conditions
                    found_prime = p; // Store found prime
                    prime_found = true; // Mark that we found a prime
                }
            }
        }
    };

    vector<thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(worker); // Create worker threads
    }

    // Wait for all threads to finish
    for (auto& t : threads) {
        t.join();
    }

    return found_prime; // Return the found prime number
}
// Tính GCD
ZZ gcd(ZZ a, ZZ b) {
    while (b != 0) {
        ZZ t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Tính lũy thừa modulo
ZZ power_mod(ZZ base, ZZ exp, ZZ mod) {
    ZZ result = ZZ(1);
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) { // Nếu exp lẻ
            result = (result * base) % mod;
        }
        exp = exp >> 1; // Chia exp cho 2
        base = (base * base) % mod; // Bình phương base
    }
    return result;
}

// Tạo khóa RSA
void generate_keypair(ZZ& n, ZZ& e, ZZ& d) {
    ZZ p = random_prime(512);
    ZZ q = random_prime(512);
    n = p * q;

    ZZ phi_n = (p - 1) * (q - 1);

    // Chọn e
    e = ZZ(65537); // Số nguyên tố nhỏ
    while (gcd(e, phi_n) != 1) {
        e++;
    }

    // Tính d
    d = InvMod(e, phi_n);
}

// Mã hóa
ZZ encrypt(const ZZ& m, const ZZ& e, const ZZ& n) {
    return power_mod(m, e, n);
}

// Giải mã
ZZ decrypt(const ZZ& c, const ZZ& d, const ZZ& n) {
    return power_mod(c, d, n);
}

int main() {
    ZZ n, e, d;
    generate_keypair(n, e, d);

    // Mã hóa một thông điệp
    ZZ m = conv<ZZ>(9); // Thông điệp
    cout << "Original message: " << m << endl;

    ZZ c = encrypt(m, e, n);
    cout << "Encrypted message: " << c << endl;

    // Giải mã thông điệp
    ZZ decrypted_m = decrypt(c, d, n);
    cout << "Decrypted message: " << decrypted_m << endl;

    return 0;
}
