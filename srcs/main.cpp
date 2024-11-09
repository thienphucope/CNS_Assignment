#include "RSA.hpp"

int main(int argc, char *argv[]) {
        
        if (argc != 2) {
        cout<<"Usage: "<<argv[0]<<" <message>\n";
        return -1;
    }


        cout<<"test: ";
                for (int i = 0; i< 10; i++){
        ZZ a = random_prime(8);
        cout<<a<<" ";
                }
    ZZ n, e, d;
    generate_keypair(n, e, d);

    // Mã hóa một thông điệp
    // Original -> ToNumber -> Encrypt -> Decrypt -> ToString -> Original
    cout << "Original message: " << argv[1] << endl;
    ZZ ToNumber = conv<ZZ>(stringToNumberUTF8(argv[1])); 
    ZZ encrypted_m = encrypt(ToNumber, e, n);
    cout << "Encrypted message: " << encrypted_m << endl;

    // Giải mã thông điệp
    ZZ decrypted_m = decrypt(encrypted_m, d, n);
    string ToString = numberToStringUTF8(decrypted_m);
    cout << "Decrypted message: " << ToString << endl;
}
