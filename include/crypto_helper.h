#ifndef CRYPTO_HELPER_H
#define CRYPTO_HELPER_H

#include <Arduino.h>
#include <mbedtls/aes.h>
#include <Base64.h>

// AES-128 yêu cầu key 16 ký tự (16 bytes)
#define AES_KEY "1234567890123456" 
// Initialization Vector (IV) - 16 ký tự
#define AES_IV  "abcdefghijklmnop" 

String encryptMessage(const String& plainText);

#endif