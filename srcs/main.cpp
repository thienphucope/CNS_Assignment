#include "RSA.hpp"

int main() {
        cout<<"test: ";
                for (int i = 0; i< 10; i++){
        ZZ a = random_prime(8);
        cout<<a<<" ";
                }
    ZZ n, e, d;
    generate_keypair(n, e, d);

    // Mã hóa một thông điệp
    ZZ m = conv<ZZ>(852025875); // Thông điệp
    cout << "Original message: " << m << endl;

    ZZ c = encrypt(m, e, n);
    cout << "Encrypted message: " << c << endl;

    // Giải mã thông điệp
    ZZ decrypted_m = decrypt(c, d, n);
    cout << "Decrypted message: " << decrypted_m << endl;
}
