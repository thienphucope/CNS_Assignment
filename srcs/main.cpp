#include "RSA.hpp"

void saveKeyToFile(const string& filename, const ZZ& n, const ZZ& key) {
    ofstream keyFile(filename);
    if (!keyFile) {
        cerr << "Lỗi: Không thể mở file " << filename << " để ghi." << endl;
        return;
    }
    keyFile << n << endl;
    keyFile << key << endl;
    keyFile.close();
    cout << "Đã lưu khóa vào " << filename << endl;
}

pair<ZZ, ZZ> loadKeyFromFile(const string& filename) {
    ifstream keyFile(filename);
    if (!keyFile) {
        cerr << "Lỗi: Không thể mở file " << filename << " để đọc." << endl;
        exit(1);
    }
    ZZ n, key;
    keyFile >> n >> key;
    keyFile.close();
    return {n, key};
}

void encryptFile() {
    string message, publicKeyPath, cipherPath;
    int keyLength;

    // Nhập thông điệp
    cout << "Nhập thông điệp để mã hoá: ";
    getline(cin, message);

    // Nhập độ dài khóa
    while (true) {
        cout << "Nhập độ dài khóa (khuyến nghị 1024, 2048, 4096): ";
        cin >> keyLength;
        if (keyLength >= 1024 && keyLength <= 4096) break;
        cout << "Độ dài khóa không hợp lệ. Vui lòng nhập lại." << endl;
    }
    cin.ignore(); // Xóa bộ đệm

    // Lựa chọn sinh khóa hoặc nhập khóa
    char choice;
    cout << "Sinh cặp khóa mới? (y/n): ";
    cin >> choice;
    cin.ignore(); // Xóa bộ đệm

    ZZ n, e, d;
    if (choice == 'y' || choice == 'Y') {
        // Sinh khóa mới
        generate_keypair(n, e, d, keyLength);
        
        // Lưu khóa công khai và riêng tư
        saveKeyToFile("public.key", n, e);
        saveKeyToFile("private.key", n, d);
    } else {
        // Nhập đường dẫn khóa công khai
        cout << "Nhập đường dẫn file public key (Enter để dùng mặc định): ";
        getline(cin, publicKeyPath);
        if (publicKeyPath.empty()) publicKeyPath = "public.key";

        // Kiểm tra tồn tại file
        if (!fs::exists(publicKeyPath)) {
            cerr << "File public key không tồn tại!" << endl;
            return;
        }

        // Nhập khóa từ file
        auto publicKey = loadKeyFromFile(publicKeyPath);
        n = publicKey.first;
        e = publicKey.second;
    }

    // Nhập đường dẫn lưu cipher
    cout << "Nhập đường dẫn lưu file cipher (Enter để dùng mặc định cipher.txt): ";
    getline(cin, cipherPath);
    if (cipherPath.empty()) cipherPath = "cipher.txt";

    // Mã hóa
    ZZ m = stringToNumber(message);
    ZZ c = encrypt(m, e, n);

    // Lưu cipher
    ofstream cipherFile(cipherPath);
    if (!cipherFile) {
        cerr << "Lỗi: Không thể mở " << cipherPath << " để ghi." << endl;
        return;
    }
    cipherFile << c;
    cipherFile.close();

    cout << "Thông điệp đã được mã hóa và lưu tại " << cipherPath << endl;
    cout << "Thông điệp mã hóa: " << c << endl;
}

void decryptFile() {
    string privateKeyPath, cipherPath;

    // Nhập đường dẫn file private key
    cout << "Nhập đường dẫn file private key (Enter để dùng mặc định): ";
    getline(cin, privateKeyPath);
    if (privateKeyPath.empty()) privateKeyPath = "private.key";

    // Kiểm tra tồn tại file
    if (!fs::exists(privateKeyPath)) {
        cerr << "File private key không tồn tại!" << endl;
        return;
    }

    // Nhập đường dẫn file cipher
    cout << "Nhập đường dẫn file cipher (Enter để dùng mặc định cipher.txt): ";
    getline(cin, cipherPath);
    if (cipherPath.empty()) cipherPath = "cipher.txt";

    // Kiểm tra tồn tại file cipher
    if (!fs::exists(cipherPath)) {
        cerr << "File cipher không tồn tại!" << endl;
        return;
    }

    // Nhập khóa riêng tư
    auto privateKey = loadKeyFromFile(privateKeyPath);
    ZZ n = privateKey.first;
    ZZ d = privateKey.second;

    // Đọc cipher
    ifstream cipherFile(cipherPath);
    ZZ c;
    cipherFile >> c;
    cipherFile.close();

    // Giải mã
    ZZ decrypted_m = decrypt(c, d, n);
    string dmessage = numberToString(decrypted_m);

    cout << "Thông điệp giải mã (số): " << decrypted_m << endl;
    cout << "Thông điệp gốc: " << dmessage << endl;
}

int main() {
    int choice;
    while (true) {
        cout << "\nMenu Mã Hóa RSA:" << endl;
        cout << "1. Mã Hóa Thông Điệp" << endl;
        cout << "2. Giải Mã Thông Điệp" << endl;
        cout << "3. Thoát" << endl;
        cout << "Chọn: ";
        cin >> choice;
        cin.ignore(); // Xóa bộ đệm

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
                cout << "Lựa chọn không hợp lệ. Vui lòng thử lại." << endl;
        }
    }
    return 0;
}

