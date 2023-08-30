#ifndef verifyTOTP_h
#define verifyTOTP_h

#include <ctime>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <string>

const std::string KEY = "1234"; // First key
const std::string AUTHEN = "1020";
const unsigned int CODE_DIGIT = 6;
const unsigned int ALLOWABLE_TIMEDRIFT = 5 * 60;
const int MAX_TRIALS = 3;
const int LOCK_TIME = 10 * 60;

bool verifyDynamicPassword(const std::string& key, const std::string& inputPassword, unsigned int adjustedTimeSteps) {
    // Convert adjusted time steps to a byte array (big-endian)
    unsigned char msg[4]; // 4 bytes for a 32-bit integer
    for (int i = 3; i >= 0; --i) {
        msg[i] = static_cast<unsigned char>(adjustedTimeSteps & 0xFF);
        adjustedTimeSteps >>= 8;
    }

    // Calculate the HMAC-SHA256 hash of the message using the first key
    unsigned char hmacResult1[32];
    HMAC(EVP_sha256(), KEY.c_str(), KEY.length(), msg, sizeof(msg), hmacResult1, nullptr);

    // Calculate the HMAC-SHA256 hash of the message using the second key
    unsigned char hmacResult2[32];
    HMAC(EVP_sha256(), key.c_str(), key.length(), msg, sizeof(msg), hmacResult2, nullptr);

    // Combine the two HMAC results using XOR
    unsigned char combinedResult[32];
    for (int i = 0; i < 32; ++i) {
        combinedResult[i] = hmacResult1[i] ^ hmacResult2[i];
    }

    // Determine the offset of the dynamic password from the combined result
    int offset = combinedResult[31] & 0x0F;

    // Extract 4 bytes from the combined result starting at the given offset
    int binaryCode = (combinedResult[offset] & 0x7F) << 24 |
                     (combinedResult[offset + 1] & 0xFF) << 16 |
                     (combinedResult[offset + 2] & 0xFF) << 8 |
                     (combinedResult[offset + 3] & 0xFF);

    // Generate the dynamic password by truncating the binaryCode to the specified number of digits
    int dynamicPassword = binaryCode % static_cast<int>(std::pow(10, CODE_DIGIT));
    
    // Determine the length of dynamic password from 4 to 6.
    int length = dynamicPassword % 3 + 4;

    // Convert the dynamic password to a string and pad with leading zeros if necessary
    std::ostringstream otpStream;
    otpStream << std::setfill('0') << std::setw(CODE_DIGIT) << dynamicPassword;
    
    std::string generatedPassword = otpStream.str();
    if (length == 5) {
        int cut_pos = generatedPassword[5] % 6;
        generatedPassword.erase(cut_pos, 1);
    } else if (length == 4) {
        int cut_pos_1 = generatedPassword[5] % 6;
        generatedPassword.erase(cut_pos_1, 1);
        int cut_pos_2 = generatedPassword[4] % 6;
        generatedPassword.erase(cut_pos_2, 1);
    }

    // Compare the generated password with the input password
    if (generatedPassword == inputPassword) {
        return true;
    }
    
    return false;
}

bool verifyTOTP(const std::string key, const std::string& inputPassword, unsigned int timeStep) {
    // Get the current time in seconds since Unix Epoch
    std::time_t currentTime = std::time(nullptr);

    // Calculate the number of time steps since Unix Epoch
    unsigned int timeSteps = static_cast<unsigned int>(currentTime / timeStep);

    // First, check for drift = 0
    if (verifyDynamicPassword(key, inputPassword, timeSteps)) {
        return true;
    }

    // Loop through positive and negative drift values
    for (int drift = 1; drift <= ALLOWABLE_TIMEDRIFT / timeStep; ++drift) {
        // Check for positive drift
        if (verifyDynamicPassword(key, inputPassword, timeSteps + drift)) {
            return true;
        }

        // Check for negative drift
        if (verifyDynamicPassword(key, inputPassword, timeSteps - drift)) {
            return true;
        }
    }

    return false;
}

#endif /* verifyTOTP_h */
