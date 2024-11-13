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
    string message = "Hello World";
    ZZ m = stringToNumber(message);
    
    cout << "Original message: " << m << endl;

    ZZ c = encrypt(m, e, n);
    cout << "Encrypted message: " << c << endl;

    // Giải mã thông điệp
    ZZ decrypted_m = decrypt(c, d, n);
    string dmessage = numberToString(decrypted_m);
    cout << "Decrypted message: " << decrypted_m << endl;
    cout << "message" << dmessage;
}
