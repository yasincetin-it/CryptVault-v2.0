#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <limits>
#include <climits>
#include <cctype>
#include <algorithm>

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/crypto.h>

#ifdef _WIN32
#include <cstdlib>
#endif

using namespace std;

// AES-256-GCM configuration
const int SALT_SIZE = 16;
const int NONCE_SIZE = 12;
const int KEY_SIZE = 32;
const int TAG_SIZE = 16;
const int PBKDF2_ITERATIONS = 600000;

const string MAGIC = "SC1";

// Display CRYPTVAULT banner with GitHub username
void showBanner() {
    cout << "\033[36m";
    cout << R"(
      ██████╗██████╗ ██╗   ██╗██████╗ ████████╗██╗   ██╗ █████╗ ██╗   ██╗██╗  ████████╗
     ██╔════╝██╔══██╗╚██╗ ██╔╝██╔══██╗╚══██╔══╝██║   ██║██╔══██╗██║   ██║██║  ╚══██╔══╝
     ██║     ██████╔╝ ╚████╔╝ ██████╔╝   ██║   ██║   ██║███████║██║   ██║██║     ██║   
     ██║     ██╔══██╗  ╚██╔╝  ██╔═══╝    ██║   ██║   ██║██╔══██║██║   ██║██║     ██║   
     ╚██████╗██║  ██║   ██║   ██║        ██║   ╚██████╔╝██║  ██║╚██████╔╝███████╗██║   
      ╚═════╝╚═╝  ╚═╝   ╚═╝   ╚═╝        ╚═╝    ╚═════╝ ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚═╝   

                        [ CRYPTVAULT AES-256-GCM ]
                        [ GITHUB: @yasincetin-it ]
      ========================================================================
)";
    cout << "\033[0m\n";
}

// Convert binary data to hexadecimal
string toHex(const unsigned char* data, size_t length) {
    stringstream ss;
    ss << hex << uppercase << setfill('0');

    for (size_t i = 0; i < length; ++i) {
        ss << setw(2) << static_cast<unsigned int>(data[i]);
    }

    return ss.str();
}

// Convert hexadecimal to binary
bool fromHex(const string& hex, vector<unsigned char>& result) {
    string cleanHex;

    for (unsigned char c : hex) {
        if (isspace(c)) {
            continue;
        }

        if (!isxdigit(c)) {
            return false;
        }

        cleanHex += static_cast<char>(c);
    }

    if (cleanHex.size() % 2 != 0) {
        return false;
    }

    result.clear();
    result.reserve(cleanHex.size() / 2);

    for (size_t i = 0; i < cleanHex.size(); i += 2) {
        unsigned int value = 0;

        string part = cleanHex.substr(i, 2);
        stringstream ss(part);
        ss >> std::hex >> value;

        if (ss.fail() || value > 255) {
            return false;
        }

        result.push_back(
            static_cast<unsigned char>(value)
        );
    }

    return true;
}

// Derive a 256-bit encryption key from the password
bool deriveKey(
    const string& password,
    const unsigned char* salt,
    unsigned char* key
) {
    if (password.empty() || password.size() > INT_MAX) {
        return false;
    }

    return PKCS5_PBKDF2_HMAC(
        password.data(),
        static_cast<int>(password.size()),
        salt,
        SALT_SIZE,
        PBKDF2_ITERATIONS,
        EVP_sha256(),
        KEY_SIZE,
        key
    ) == 1;
}

// Encrypt using AES-256-GCM
bool encryptMessage(
    const string& message,
    const string& password,
    string& encryptedHex
) {
    if (message.size() > INT_MAX) {
        return false;
    }

    unsigned char salt[SALT_SIZE] = {};
    unsigned char nonce[NONCE_SIZE] = {};
    unsigned char key[KEY_SIZE] = {};
    unsigned char tag[TAG_SIZE] = {};

    bool success = false;
    EVP_CIPHER_CTX* ctx = nullptr;

    vector<unsigned char> ciphertext(message.size() + 16);

    if (RAND_bytes(salt, SALT_SIZE) != 1 ||
        RAND_bytes(nonce, NONCE_SIZE) != 1) {
        goto cleanup;
    }

    if (!deriveKey(password, salt, key)) {
        goto cleanup;
    }

    ctx = EVP_CIPHER_CTX_new();

    if (!ctx) {
        goto cleanup;
    }

    {
        int length = 0;
        int totalLength = 0;

        if (EVP_EncryptInit_ex(
            ctx, EVP_aes_256_gcm(),
            nullptr, nullptr, nullptr) != 1) {
            goto cleanup;
        }

        if (EVP_CIPHER_CTX_ctrl(
            ctx, EVP_CTRL_GCM_SET_IVLEN,
            NONCE_SIZE, nullptr) != 1) {
            goto cleanup;
        }

        if (EVP_EncryptInit_ex(
            ctx, nullptr, nullptr,
            key, nonce) != 1) {
            goto cleanup;
        }

        if (EVP_EncryptUpdate(
            ctx,
            ciphertext.data(),
            &length,
            reinterpret_cast<const unsigned char*>(
                message.data()
                ),
            static_cast<int>(message.size())) != 1) {
            goto cleanup;
        }

        totalLength = length;

        if (EVP_EncryptFinal_ex(
            ctx,
            ciphertext.data() + totalLength,
            &length) != 1) {
            goto cleanup;
        }

        totalLength += length;
        ciphertext.resize(
            static_cast<size_t>(totalLength)
        );

        if (EVP_CIPHER_CTX_ctrl(
            ctx, EVP_CTRL_GCM_GET_TAG,
            TAG_SIZE, tag) != 1) {
            goto cleanup;
        }
    }

    {
        // Packet: magic + salt + nonce + ciphertext + tag
        vector<unsigned char> packet;

        packet.insert(
            packet.end(), MAGIC.begin(), MAGIC.end()
        );

        packet.insert(
            packet.end(), salt, salt + SALT_SIZE
        );

        packet.insert(
            packet.end(), nonce, nonce + NONCE_SIZE
        );

        packet.insert(
            packet.end(),
            ciphertext.begin(), ciphertext.end()
        );

        packet.insert(
            packet.end(), tag, tag + TAG_SIZE
        );

        encryptedHex = toHex(
            packet.data(), packet.size()
        );
    }

    success = true;

cleanup:
    if (ctx) {
        EVP_CIPHER_CTX_free(ctx);
    }

    OPENSSL_cleanse(key, sizeof(key));
    OPENSSL_cleanse(tag, sizeof(tag));

    return success;
}

// Decrypt and authenticate using AES-256-GCM
bool decryptMessage(
    const string& encryptedHex,
    const string& password,
    string& originalMessage
) {
    vector<unsigned char> packet;

    if (!fromHex(encryptedHex, packet)) {
        return false;
    }

    const size_t headerSize =
        MAGIC.size() + SALT_SIZE + NONCE_SIZE;

    if (packet.size() < headerSize + TAG_SIZE) {
        return false;
    }

    if (!equal(
        MAGIC.begin(), MAGIC.end(),
        packet.begin())) {
        return false;
    }

    const unsigned char* salt =
        packet.data() + MAGIC.size();

    const unsigned char* nonce =
        salt + SALT_SIZE;

    const unsigned char* ciphertext =
        nonce + NONCE_SIZE;

    const size_t ciphertextSize =
        packet.size() - headerSize - TAG_SIZE;

    if (ciphertextSize > INT_MAX) {
        return false;
    }

    const unsigned char* tag =
        packet.data() + packet.size() - TAG_SIZE;

    unsigned char key[KEY_SIZE] = {};
    bool success = false;

    EVP_CIPHER_CTX* ctx = nullptr;

    vector<unsigned char> plaintext(
        ciphertextSize + 16
    );

    if (!deriveKey(password, salt, key)) {
        OPENSSL_cleanse(key, sizeof(key));
        return false;
    }

    ctx = EVP_CIPHER_CTX_new();

    if (!ctx) {
        OPENSSL_cleanse(key, sizeof(key));
        return false;
    }

    {
        int length = 0;
        int totalLength = 0;

        if (EVP_DecryptInit_ex(
            ctx, EVP_aes_256_gcm(),
            nullptr, nullptr, nullptr) != 1) {
            goto cleanup;
        }

        if (EVP_CIPHER_CTX_ctrl(
            ctx, EVP_CTRL_GCM_SET_IVLEN,
            NONCE_SIZE, nullptr) != 1) {
            goto cleanup;
        }

        if (EVP_DecryptInit_ex(
            ctx, nullptr, nullptr,
            key, nonce) != 1) {
            goto cleanup;
        }

        if (EVP_DecryptUpdate(
            ctx,
            plaintext.data(),
            &length,
            ciphertext,
            static_cast<int>(ciphertextSize)) != 1) {
            goto cleanup;
        }

        totalLength = length;

        if (EVP_CIPHER_CTX_ctrl(
            ctx, EVP_CTRL_GCM_SET_TAG,
            TAG_SIZE,
            const_cast<unsigned char*>(tag)) != 1) {
            goto cleanup;
        }

        // Authentication fails if the password or data is incorrect
        if (EVP_DecryptFinal_ex(
            ctx,
            plaintext.data() + totalLength,
            &length) != 1) {
            goto cleanup;
        }

        totalLength += length;

        plaintext.resize(
            static_cast<size_t>(totalLength)
        );

        originalMessage.assign(
            reinterpret_cast<const char*>(
                plaintext.data()
                ),
            plaintext.size()
        );

        success = true;
    }

cleanup:
    EVP_CIPHER_CTX_free(ctx);
    OPENSSL_cleanse(key, sizeof(key));

    if (!success) {
        originalMessage.clear();
    }

    return success;
}

int main() {
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    showBanner();

    cout << "\033[33m[1]\033[0m Encrypt Message\n";
    cout << "\033[33m[2]\033[0m Decrypt Message\n";
    cout << "Your choice: ";

    int choice;

    if (!(cin >> choice)) {
        cout << "Invalid selection!\n";
        return 1;
    }

    cin.ignore(
        numeric_limits<streamsize>::max(), '\n'
    );

    string message;
    string password;
    string result;

    if (choice == 1) {
        cout << "\n[?] Enter message to encrypt:\n";
        getline(cin, message);

        cout << "[?] Enter a strong password: ";
        getline(cin, password);

        if (encryptMessage(message, password, result)) {
            cout << "\n\033[32m"
                << "[+] AES-256-GCM encryption successful!"
                << "\033[0m\n";

            cout << "Encrypted Hex message:\n"
                << result << "\n";
        }
        else {
            cout << "\n\033[31m"
                << "[-] Encryption failed!"
                << "\033[0m\n";
        }
    }
    else if (choice == 2) {
        cout << "\n[?] Enter encrypted Hex message:\n";
        getline(cin, message);

        cout << "[?] Enter password: ";
        getline(cin, password);

        if (decryptMessage(message, password, result)) {
            cout << "\n\033[32m"
                << "[+] Message authenticated and decrypted!"
                << "\033[0m\n";

            cout << "Original message:\n"
                << result << "\n";
        }
        else {
            cout << "\n\033[31m"
                << "[-] Decryption failed!"
                << "\033[0m\n";

            cout << "Incorrect password, invalid data, "
                << "or corrupted message.\n";
        }
    }
    else {
        cout << "\033[31mInvalid selection!\033[0m\n";
    }

    OPENSSL_cleanse(
        password.data(), password.size()
    );

#ifdef _WIN32
    cout << "\n";
    system("pause");
#endif

    return 0;
}