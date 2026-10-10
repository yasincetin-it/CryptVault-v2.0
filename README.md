# CryptVault v2.0 🔒

![Language](https://img.shields.io/badge/Language-C%2B%2B20-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Windows%20(CNG)-0078D6.svg)
![Algorithm](https://img.shields.io/badge/Cipher-AES--256--GCM-green.svg)
![License](https://img.shields.io/badge/License-MIT-brightgreen.svg)

**CryptVault v2.0** is a lightweight, high-performance, command-line text encryption and decryption tool engineered in C++20 for Windows environments. It leverages native Windows Cryptography API: Next Generation (**CNG / BCrypt**) to provide hardware-accelerated **AES-256-GCM** authenticated encryption for absolute confidentiality and data integrity.

---

## 🌟 Key Features

- **AES-256-GCM Encryption:** Ensures strong data confidentiality along with built-in message integrity checking (Authenticated Encryption).
- **PBKDF2 Key Derivation:** Uses PBKDF2 with 100,000 iterations and a uniquely generated 16-byte salt to derive strong 256-bit cryptographic keys from user passwords.
- **Native Windows API:** Utilizes Windows CNG (`bcrypt.dll`) directly without external third-party library dependencies.
- **Randomized Nonce/Salt:** Generates high-entropy Cryptographic Random Numbers (RNG) for IV and Salt per encryption operation.
- **Zero Third-Party Dependencies:** Pure C++ implementation optimized for standard Windows platforms.

---

## 🏗️ Technical Architecture & Format

When text is encrypted, CryptVault produces an output binary/hex structure formatted as follows:

| Offset / Field | Size | Description |
| :--- | :--- | :--- |
| **Salt Size** | `uint32_t` (4 bytes) | Length of PBKDF2 Salt |
| **Salt Data** | Variable (16 bytes) | Cryptographic Salt for Key Derivation |
| **IV Size** | `uint32_t` (4 bytes) | Length of Initialization Vector / Nonce |
| **IV Data** | Variable (12 bytes) | GCM Initialization Vector |
| **Tag Size** | `uint32_t` (4 bytes) | Authentication Tag Length |
| **Tag Data** | Variable (16 bytes) | GCM Authentication Tag for integrity check |
| **Cipher Size** | `uint32_t` (4 bytes) | Length of Encrypted Content |
| **Ciphertext** | Variable | Authenticated Encrypted Payload Data |

---

## 🚀 Getting Started

### Prerequisites

- **OS:** Windows 10 / 11
- **Compiler:** MSVC (Visual Studio 2022 recommended) supporting **C++20**
- **Libraries:** Windows SDK (`bcrypt.lib`)

### Building the Project

1. Clone the repository:
   `git clone [https://github.com/yasincetin-it/CryptVault-v2.0.git](https://github.com/yasincetin-it/CryptVault-v2.0.git)`
   `cd CryptVault-v2.0`
2. Open `CryptVaultV2.0.sln` in **Visual Studio**.
3. Set configuration to `Release` and architecture to `x64`.
4. Build the solution (`Ctrl + Shift + B`).

---

## 💻 Usage

CryptVault is operated directly via Command Prompt (`cmd.exe`) or PowerShell.

### Encrypting Text
Command: `CryptVaultV2.0.exe encrypt <text> <password>`

*Example:*
`CryptVaultV2.0.exe encrypt "GizliMetin123" "MySecretPassword123"`

### Decrypting Text
Command: `CryptVaultV2.0.exe decrypt <encrypted_data> <password>`

*Example:*
`CryptVaultV2.0.exe decrypt "<encrypted_string>" "MySecretPassword123"`

> Restores original text upon password validation.

---

## 🛡️ Security Considerations

- **Password Strength:** CryptVault relies on user passwords for PBKDF2 key generation. Use complex passwords to prevent brute-force resistance.
- **Integrity Validation:** If the encrypted payload is altered or an incorrect password is provided during decryption, the GCM tag validation will fail, protecting against ciphertext tampering.

---

## 📜 License

This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for full details.
