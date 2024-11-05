#include "RSA.hpp"

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



bool MillerRabinTest(ZZ to_test) {
    if (to_test < 2) return false;   // 0 và 1 không phải số nguyên tố
    if (to_test != 2 && to_test % 2 == 0) return false; // Số chẵn > 2 không phải số nguyên tố

    // Lấy s và d từ to_test - 1 = 2^s * d
    ZZ d = to_test - 1;
    ZZ s = conv<ZZ>(0);
    while (d % 2 == 0) {
        d /= 2;
        s++;
    }

    // Kiểm tra với một số lượng nhất định các cơ số
    constexpr int accuracy = 20; // Số lượng kiểm tra
   // std::random_device rd;
   // std::mt19937 gen(rd());
   // std::uniform_int_distribution<uint64_t> distr(2, to_test - 2); // Random a trong khoảng [2, to_test - 2]

    for (int i = 0; i < accuracy; i++) {
       // uint64_t a = distr(gen);
        ZZ a = RandomBnd(to_test - 1) + 2;
        ZZ x = power_mod(a, d, to_test); // x = a^d mod to_test

        if (x == 1 || x == to_test - 1)
            continue; // Nếu x là 1 hoặc to_test - 1, tiếp tục với lần kiểm tra tiếp theo

        bool composite = true; // Giả định to_test là hợp số
        for (uint64_t j = 0; j < s - 1; j++) {
            x = power_mod(x, ZZ(2), to_test); // Tính x = x^2 mod to_test
            if (x == to_test - 1) {
                composite = false; // x là bội số của to_test - 1, không phải hợp số
                break; // Thoát vòng lặp
            }
        }

        if (composite) {
            return false; // Nếu không tìm thấy bội số của to_test - 1, trả về false (hợp số)
        }
    }
    
    return true; // Nếu không có chứng minh nào về tính hợp số, trả về true (nguyên tố)
}

ZZ random_prime(int bit_length) {
    ZZ p;
    while (true) {
        p = RandomBits_ZZ(bit_length);
        if (MillerRabinTest(p)) return p;
    
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


