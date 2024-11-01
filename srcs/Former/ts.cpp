#include <iostream>
#include <NTL/ZZ.h>
#include <gmp.h>
#include <ctime>

using namespace std;
using namespace NTL;

// Hàm kiểm tra số nguyên tố
bool is_prime(const ZZ& n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (even(n)) return false;
    for (ZZ i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

// Sinh số nguyên tố ngẫu nhiên
ZZ random_prime(int bit_length) {
    ZZ p;
    while (true) {
        p = RandomBnd(power(ZZ(2), bit_length));
        if (is_prime(p)) return p;
    }
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
    ZZ result = 1;
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