#ifndef generateTOTP_h
#define generateTOTP_h

#include <ctime>
#include <iomanip>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <cmath>
#include <sstream>
#include <string>

const unsigned int CODE_DIGIT = 6;

std::string generateTOTP(const std::string& key1, const std::string& key2, unsigned int timeStep, unsigned int codeDigits = 6) {
    // Get the current time in seconds since Unix Epoch
    std::time_t currentTime = std::time(nullptr);

    // Calculate the number of time steps since Unix Epoch
    unsigned int timeSteps = static_cast<unsigned int>(currentTime / timeStep);

    // Convert time steps to a byte array (big-endian)
    unsigned char msg[4]; // 4 bytes for a 32-bit integer
    for (int i = 3; i >= 0; --i) {
        msg[i] = static_cast<unsigned char>(timeSteps & 0xFF);
        timeSteps >>= 8;
    }

    // Calculate the HMAC-SHA256 hash of the message using the first key
    unsigned char hmacResult1[32];
    HMAC(EVP_sha256(), key1.c_str(), key1.length(), msg, sizeof(msg), hmacResult1, nullptr);

    // Calculate the HMAC-SHA256 hash of the message using the second key
    unsigned char hmacResult2[32];
    HMAC(EVP_sha256(), key2.c_str(), key2.length(), msg, sizeof(msg), hmacResult2, nullptr);

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
    int dynamicPassword = binaryCode % static_cast<int>(std::pow(10, codeDigits));
    
    // Determine the length of dynamic password from 4 to 6.
    int length = dynamicPassword % 3 + 4;

    // Convert the dynamic password to a string and pad with leading zeros if necessary
    std::ostringstream otpStream;
    otpStream << std::setfill('0') << std::setw(codeDigits) << dynamicPassword;
    
    std::string output = otpStream.str();
    if (length == 5) {
        int cut_pos = output[5] % 6;
        output.erase(cut_pos, 1);
    } else if (length == 4) {
        int cut_pos_1 = output[5] % 6;
        output.erase(cut_pos_1, 1);
        int cut_pos_2 = output[4] % 6;
        output.erase(cut_pos_2, 1);
    }
    
    return output;
}

#endif /* generateTOTP_h */
