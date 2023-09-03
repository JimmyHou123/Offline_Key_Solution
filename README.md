# generateTOTP and verifyTOTP

`generateTOTP` and `verifyTOTP` are C++ programs for generating and verifying Time-Based One-Time Passwords (TOTP) based on the HOTP algorithm. These programs use OpenSSL for cryptographic operations and are designed to be easy to integrate into your applications for two-factor authentication (2FA).

## Table of Contents

- [Features](#features)
- [Prerequisites](#prerequisites)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Examples](#examples)
- [License](#license)

## Features

- Generate TOTP tokens based on a shared secret key.
- Verify TOTP tokens for authentication.
- Adjustable time step intervals.
- Allowable time drift for token verification.
- Configurable code digits (default is 6 digits).
- Automatic locking after a specified number of failed verification attempts.
- Automatic unlocking after a specified lock time.

## Prerequisites

Before using `generateTOTP` and `verifyTOTP`, you'll need the following:

- A C++ development environment.
- OpenSSL library installed on your system. You can download OpenSSL from [https://www.openssl.org/source/](https://www.openssl.org/source/).

## Getting Started

1. Clone or download the repository to your local machine.

2. Make sure you have OpenSSL installed. You may need to link against OpenSSL libraries during compilation. On Linux, you can install OpenSSL using your package manager. On macOS, you can use Homebrew (`brew install openssl`).

3. Compile the programs using a C++ compiler. For example, you can use `g++`:

   ```bash
   g++ -std=c++11 generateTOTP_main.cpp -o generateTOTP -lcrypto
   g++ -std=c++11 verifyTOTP_main.cpp -o verifyTOTP -lcrypto

## Usage

1. `generateTOTP` generates TOTP tokens based on a shared secret key. You can run the program by `./generateTOTP` and follow the instruction. Normally, secret key is the ID of the smart lock.

2. `verifyTOTP` verifies TOTP tokens for authentication. It can be used to verify tokens provided by users during the authentication process. You can run the program by "./verifyTOTP" and follow the instruction. Normally, authentication password is the initial password.

## Example

./generateTOTP
Choose a time step option:
1. 1 minute
2. 15 minutes
3. 1 hour
4. 12 hours
5. 24 hours
2
Dynamic Password with 15 minutes time step: 7627

./verifyTOTP
Choose a time step option:
1. 1 minute
2. 15 minutes
3. 1 hour
4. 12 hours
5. 24 hours
2
Enter the password to verify: 7627
Password is valid.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
