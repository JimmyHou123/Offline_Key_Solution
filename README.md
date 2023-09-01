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
   g++ -std=c++11 generateTOTP.cpp -o generateTOTP -lcrypto
   g++ -std=c++11 verifyTOTP.cpp -o verifyTOTP -lcrypto
