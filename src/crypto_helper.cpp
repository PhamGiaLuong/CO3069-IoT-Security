#include "crypto_helper.h"

String encryptMessage(const String& plainText) {
    // 1. Chuẩn bị Key và IV
    // Lưu ý: mbedtls sẽ thay đổi IV sau khi chạy, nên cần copy ra biến tạm
    unsigned char key[17] = AES_KEY;
    unsigned char iv[17] = AES_IV;

    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);
    mbedtls_aes_setkey_enc(&aes, key, 128); 

    // 2. Tính toán Padding (PKCS7)
    int inputLen = plainText.length();
    int remainder = inputLen % 16;
    int padding = 16 - remainder;
    int paddedLen = inputLen + padding;

    unsigned char inputBuffer[paddedLen];
    plainText.getBytes(inputBuffer, inputLen + 1);

    // Thêm byte đệm (giá trị của byte đệm = số lượng byte cần đệm)
    for (int i = inputLen; i < paddedLen; i++) {
        inputBuffer[i] = (unsigned char)padding;
    }

    // 3. Mã hóa AES
    unsigned char outputBuffer[paddedLen];
    mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, paddedLen, iv, inputBuffer, outputBuffer);
    mbedtls_aes_free(&aes);

    // 4. Chuyển sang Base64 để gửi qua MQTT an toàn
    int encodedLen = Base64.encodedLength(paddedLen);
    char encodedString[encodedLen + 1];
    Base64.encode(encodedString, (char*)outputBuffer, paddedLen);
    
    return String(encodedString);
}