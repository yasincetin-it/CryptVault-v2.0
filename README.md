# 🔐 CryptVault v2.0

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)
![Security](https://img.shields.io/badge/Encryption-AES--256--GCM-red.svg)

**CryptVault v2.0** is a high-performance, lightweight, and secure command-line file encryption/decryption tool built for Windows environments. It utilizes modern C++20 features alongside the native **Windows Cryptography API: Next Generation (CNG / BCrypt)** to deliver authenticated, enterprise-grade encryption.

---

## 🌟 Key Features

- **🔒 Authenticated Encryption (AES-256-GCM):** Guarantees both confidentiality and data integrity. Any unauthorized tampering or corruption in the ciphertext immediately fails the decryption process.
- **🛡️ Robust Key Derivation (PBKDF2-HMAC-SHA256):** Converts user-provided passwords into cryptographic keys using PBKDF2 with 100,000 iterations and a unique salt per file.
- **⚡ Zero External Dependencies:** Built completely with native C++ and Windows CNG (BCrypt) libraries. No Third-party libraries (e.g., OpenSSL) required.
- **💾 Low Memory Footprint:** Efficient stream-based chunk processing allows handling large files smoothly without loading the entire file into RAM.
- **🎯 User-Friendly CLI:** Clear console workflow with built-in validation for paths and input parameters.

---

## 📐 Cryptographic Architecture

| Component | Specification |
| :--- | :--- |
| **Cipher Algorithm** | AES (Advanced Encryption Standard) |
| **Cipher Mode** | GCM (Galois/Counter Mode) |
| **Key Size** | 256 bits |
| **Auth Tag Size** | 128 bits (16 bytes) |
| **KDF Algorithm** | PBKDF2 with HMAC-SHA256 |
| **KDF Iterations** | 100,000 rounds |
| **IV / Nonce Size** | 96 bits (12 bytes, randomly generated per operation) |

---

## 🛠️ Building & Installation

### Prerequisites
* **Operating System:** Windows 10 / 11 (64-bit)
* **Compiler:** MSVC (Microsoft Visual C++ Compiler with C++20 support)
* **IDE:** Visual Studio 2022 or higher

### Build Steps

1. Clone the repository:
   ```bash
   git clone [https://github.com/yasincetin-it/CryptVault-v2.0.git](https://github.com/yasincetin-it/CryptVault-v2.0.git)
   cd CryptVault-v2.0