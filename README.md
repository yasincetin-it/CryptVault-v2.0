# 🔐 CryptVault v2.0

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)
![Security](https://img.shields.io/badge/Encryption-AES--256--GCM-red.svg)

CryptVault v2.0 is an educational password-based encryption and decryption tool written in C++ for Windows. It uses AES-256-GCM for authenticated encryption and PBKDF2-HMAC-SHA256 to derive a key from a user password.

This project is intended for learning, experimentation, and understanding the fundamentals of authenticated encryption, key derivation, and secure message handling in C++.

> Note: This repository currently focuses on encrypting and decrypting text messages. File encryption support is a planned future enhancement.

---

## ✨ Features

- AES-256-GCM authenticated encryption
- Password-based key derivation using PBKDF2-HMAC-SHA256
- Unique random salt and nonce generated per encryption
- Auth tag validation for tamper detection
- Hex-encoded encrypted output
- Simple Windows CLI workflow
- Educational and prototype-oriented design

---

## 🔐 Cryptographic Architecture

| Component | Specification |
| :--- | :--- |
| Cipher | AES |
| Mode | GCM (Galois/Counter Mode) |
| Key Size | 256 bits |
| Authentication Tag | 128 bits |
| KDF | PBKDF2-HMAC-SHA256 |
| Iterations | 600,000 |
| Salt Size | 16 bytes |
| Nonce / IV Size | 12 bytes |

This design follows the standard pattern for authenticated encryption:
- Password -> PBKDF2 -> encryption key
- Random salt + random nonce
- AES-GCM encryption
- Authentication tag verification during decryption

---

## 🧠 Current Scope

This project currently demonstrates how to:
- encrypt a text message
- derive a key from a password
- generate a random salt and nonce
- authenticate data using GCM tag verification
- decrypt the message only if the password is correct

It is a learning-focused implementation and should not be treated as a production-grade secure vault or enterprise encryption system.

---

## 🛠️ Requirements

### Prerequisites
- Windows 10 / 11
- Visual Studio 2022 or newer
- C++20-capable compiler
- OpenSSL development libraries

### Required Libraries
- OpenSSL `libcrypto`
- `openssl/evp.h`
- `openssl/rand.h`
- `openssl/crypto.h`

---

## 🚀 Building

1. Clone the repository:
   ```bash
   git clone https://github.com/yasincetin-it/CryptVault-v2.0.git
   cd CryptVault-v2.0
   ```

2. Open the Visual Studio solution/project:
   - `CryptVaultV2.0.slnx`
   - or the generated Visual Studio project file

3. Make sure OpenSSL is installed and configured correctly for the project.

4. Build the solution in Debug or Release mode.

---

## ▶️ Usage

Run the executable and choose one of the available options:

### 1. Encrypt Message
- Enter the message to encrypt
- Enter a password
- The program outputs a hex-encoded encrypted message

### 2. Decrypt Message
- Paste the encrypted hex output
- Enter the same password
- If correct, the original message is recovered

Example flow:
```text
[1] Encrypt Message
[2] Decrypt Message
Your choice: 1

[?] Enter message to encrypt:
Hello world

[?] Enter a strong password:
MySecurePass123!

[+] AES-256-GCM encryption successful!
Encrypted Hex message:
...
```

---

## ⚠️ Security Note

This project is an educational prototype and is not intended for production use without additional hardening.

Important considerations:
- use strong, unique passwords
- never store passwords in plaintext
- use unique nonces for every encryption
- validate input carefully
- use secure storage for keys and metadata
- do not use this project for sensitive or regulated data without a proper security review

---

## 📌 Project Goals

The goal of this project is to learn and practice:
- authenticated encryption
- key derivation
- secure random generation
- modern C++ programming
- Windows-native development
- GitHub project structure and documentation

---

## 🤝 License

This project is licensed under the MIT License. See the `LICENSE` file for details.

---

## 👨‍💻 Author

GitHub: @yasincetin-it

---

## 📝 Future Improvements

Planned improvements include:
- file encryption / decryption support
- better password validation
- structured project layout (src/include)
- unit tests
- stronger CLI validation
- GUI or web-based frontend
- CI/CD support
- improved documentation and security guidelines
