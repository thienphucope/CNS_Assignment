#include <iostream>
#include <cstdint>
#include <random>

// Hàm tính (base^exp) mod mod
uint64_t powMod(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base = base % mod;  // Tránh overflow

    while (exp > 0) {
        // Nếu exp là lẻ, nhân với base
        if (exp % 2 == 1)
            result = (result * base) % mod;
        
        // exp phải là số chẵn
        exp = exp >> 1;  // exp / 2
        base = (base * base) % mod;  // base^2
    }
    return result;
}

// Hàm kiểm tra tính nguyên tố bằng Miller-Rabin
bool MillerRabinTest(uint64_t to_test) {
    if (to_test < 2) return false;   // 0 và 1 không phải số nguyên tố
    if (to_test != 2 && to_test % 2 == 0) return false; // Số chẵn > 2 không phải số nguyên tố

    // Lấy s và d từ to_test - 1 = 2^s * d
    uint64_t d = to_test - 1;
    uint64_t s = 0;
    while (d % 2 == 0) {
        d /= 2;
        s++;
    }

    // Kiểm tra với một số lượng nhất định các cơ số
    constexpr int accuracy = 20; // Số lượng kiểm tra
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> distr(2, to_test - 2); // Random a trong khoảng [2, to_test - 2]

    for (int i = 0; i < accuracy; i++) {
        uint64_t a = distr(gen);
        uint64_t x = powMod(a, d, to_test); // x = a^d mod to_test

        if (x == 1 || x == to_test - 1)
            continue; // Nếu x là 1 hoặc to_test - 1, tiếp tục với lần kiểm tra tiếp theo

        bool composite = true; // Giả định to_test là hợp số
        for (uint64_t j = 0; j < s - 1; j++) {
            x = powMod(x, 2, to_test); // Tính x = x^2 mod to_test
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
