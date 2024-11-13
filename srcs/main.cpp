#include "RSA.hpp"

int main() {
    ZZ n, e, d;
    generate_keypair(n, e, d);

    // Mã hóa một thông điệp
    string message = "The Rain Is Ceasing But I'm Not Feeling Sweet";
    cout<<"Sent Message: "<<message<<endl<<endl;
    ZZ m = stringToNumber(message);
    
    cout << "ConvertToNumber: " << m << endl<<endl;

    ZZ c = encrypt(m, e, n);
    cout << "Encrypted Message: " << c << endl<<endl;

    // Giải mã thông điệp
    ZZ decrypted_m = decrypt(c, d, n);
    string dmessage = numberToString(decrypted_m);
    cout << "Decrypted Message: " << decrypted_m << endl<<endl;
    cout << "ConverToString -> Received Message: " << dmessage<<endl;
    return 0;
}
