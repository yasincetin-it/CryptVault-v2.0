# CryptVault v2.0 🔒

![Language](https://img.shields.io/badge/Language-C%2B%2B20-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Windows%20%2F%20Cross--Platform-0078D6.svg)
![Crypto Library](https://img.shields.io/badge/Library-OpenSSL-red.svg)
![Algorithm](https://img.shields.io/badge/Cipher-AES--256--GCM-green.svg)
![License](https://img.shields.io/badge/License-MIT-brightgreen.svg)

**CryptVault v2.0** is an interactive, menu-driven command-line text encryption and decryption tool engineered in C++20. Powered by the industry-standard **OpenSSL** library (`libcrypto`), it provides robust **AES-256-GCM** authenticated encryption for high confidentiality and tamper-proof data integrity.

---

## 🌟 Key Features

- **AES-256-GCM Authenticated Encryption:** Protects sensitive text using 256-bit AES in GCM mode, ensuring both confidentiality and message integrity validation.
- **Hardened Key Derivation (PBKDF2):** Utilizes OpenSSL's `PKCS5_PBKDF2_HMAC` with **600,000 iterations** and dynamic 16-byte cryptographic salts to generate strong keys resistant to brute-force attacks.
- **High-Entropy Randomness:** Uses OpenSSL's cryptographically secure pseudo-random number generator (`RAND_bytes`) for Initialization Vectors (IV/Nonce) and Salts.
- **Interactive CLI Interface:** User-friendly terminal menu for quick text encryption and decryption without complex CLI flag parsing.

---

## 🏗️ Technical Architecture & Binary Format

When text is encrypted, CryptVault constructs a formatted payload containing key derivation metadata, GCM parameters, and the ciphertext:

| Offset / Field | Size | Description |
| :--- | :--- | :--- |
| **Salt Size** | `uint32_t` (4 bytes) | Length of PBKDF2 Salt |
| **Salt Data** | Variable (16 bytes) | High-Entropy Salt for Key Derivation |
| **IV Size** | `uint32_t` (4 bytes) | Length of GCM Initialization Vector |
| **IV Data** | Variable (12 bytes) | GCM Nonce / IV |
| **Tag Size** | `uint32_t` (4 bytes) | Length of Authentication Tag |
| **Tag Data** | Variable (16 bytes) | GCM Authentication Tag for integrity check |
| **Cipher Size** | `uint32_t` (4 bytes) | Length of Encrypted Text |
| **Ciphertext** | Variable | Authenticated Encrypted Payload Data |

---

## 🚀 Getting Started

### Prerequisites

- **OS:** Windows 10 / 11 (MSVC recommended)
- **Compiler:** C++20 compliant compiler
- **Dependencies:** [OpenSSL](https://www.openssl.org/) C++ library (`libssl`, `libcrypto`)

### Building the Project

1. Clone the repository:
   `git clone https://github.com/yasincetin-it/CryptVault-v2.0.git`
   `cd CryptVault-v2.0`
2. Open `CryptVaultV2.0.slnx` in **Visual Studio 2022**.
3. Ensure OpenSSL includes and library directories are configured in project settings.
4. Set configuration to `Release` and architecture to `x64`.
5. Build the solution (`Ctrl + Shift + B`).

---

## 💻 Usage

Run `CryptVaultV2.0.exe` directly from Command Prompt, PowerShell, or Visual Studio.

```text
======================================
         CryptVault v2.0 - CLI       
======================================
1. Encrypt Text
2. Decrypt Text
3. Exit
Select an option:
