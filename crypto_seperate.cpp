#include <iostream>
#include <openssl/rc4.h>
#include <openssl/des.h>
#include <openssl/sha.h>

using namespace std;

// ================ RC4 Start ================================
string rc4_encrypt(string data, string secret_key) {
    unsigned char* output = (unsigned char*)malloc(data.length());
    if (data.length() == 0 || secret_key.length() == 0) return "";
    RC4_KEY key;
    RC4_set_key(&key, secret_key.length(), (const unsigned char*)secret_key.c_str());
    RC4(&key, (unsigned long)data.length(), (const unsigned char*)data.c_str(), output);
    string res((char*)output, data.length());
    return res;
}

string rc4_decrypt(string data, string secret_key) {
    unsigned char* output = (unsigned char*)malloc(data.length());
    if (data.length() == 0 || secret_key.length() == 0)return "";
    RC4_KEY key;
    RC4_set_key(&key, secret_key.length(), (const unsigned char*)secret_key.c_str());
    RC4(&key, (unsigned long)data.length(), (const unsigned char*)data.c_str(), output);
    string res((char*)output, data.length());
    return res;
}

// ================ RC4   End ================================


// ================ DES Start ================================
/*
    ivec参数是8字节的初始向量（CFB,CBC,OFB）
    使用CBC模式进行加密时；length参数实际上是string.length()
    使用CBC模式解密时；length参数为加密时(length/8 + 1) * 8;
    CFB模式的numbits参数是每次加密的比特数    
*/

void convert_string_to_des_block(string str, DES_cblock& output);

string des_encrypt(string plain, string secret_key) {
    DES_cblock key;
    DES_string_to_key((const char*)secret_key.c_str(), &key);
    DES_key_schedule keys;
    DES_set_key_checked(&key, &keys);

    DES_cblock plaintext, output;
    convert_string_to_des_block(plain, plaintext);
    DES_ecb_encrypt(&plaintext, &output, &keys, DES_ENCRYPT);
    string result((char*)output, plain.length());
    return result;
}

string des_decrypt(string cipher, string secret_key) {
    DES_cblock key;
    DES_string_to_key((const char*)secret_key.c_str(), &key);
    DES_key_schedule keys;
    DES_set_key_checked(&key, &keys);

    DES_cblock ciphertext, output;
    convert_string_to_des_block(cipher, ciphertext);
    DES_ecb_encrypt(&ciphertext, &output, &keys, DES_DECRYPT);
    string result((char*)output, cipher.length());
    return result;
}
// ================ DES   End ================================

// ================ SHA1 Start ===============================

string sha1_digest(string msg) {
   /* unsigned char buf[20];
    SHA1((const unsigned char*)msg.c_str(), msg.length(), buf);
    string res((char*)buf, 20);
    return res;*/

    SHA_CTX c;
    SHA1_Init(&c);
    unsigned char buf[20];
    SHA1_Update(&c, (unsigned char*)msg.c_str(), msg.length());
    SHA1_Final(&(buf[0]), &c);
    string res((char*)buf, 20);
    return res;
}
// ================ SHA1   End ===============================

// ================ Helper Start =============================
/*
    DES ECB模式明密文转换为DES_cblock函数
*/
void convert_string_to_des_block(string str, DES_cblock& output) {
    for (int i = 0; i < 8; ++i) {
        output[i] = str[i];
    }
}
// ================ Helper End ================================
/*
    进制转换函数
*/

void stream2hex(const std::string str, std::string& hexstr, bool capital = false) {
    hexstr.resize(str.size() * 2);
    const size_t a = capital ? 'A' - 1 : 'a' - 1;

    for (size_t i = 0, c = str[0] & 0xFF; i < hexstr.size(); c = str[i / 2] & 0xFF) {
        hexstr[i++] = c > 0x9F ? (c / 16 - 9) | a : c / 16 | '0';
        hexstr[i++] = (c & 0xF) > 9 ? (c % 16 - 9) | a : c % 16 | '0';
    }
}

// Convert string of hex numbers to its equivalent char-stream
void hex2stream(const std::string hexstr, std::string& str) {
    str.resize((hexstr.size() + 1) / 2);

    for (size_t i = 0, j = 0; i < str.size(); i++, j++) {
        str[i] = (hexstr[j] & '@' ? hexstr[j] + 9 : hexstr[j]) << 4, j++;
        str[i] |= (hexstr[j] & '@' ? hexstr[j] + 9 : hexstr[j]) & 0xF;
    }
}

// wrapper for convert string to hex
string string2hex(const string str) {
    string s;
    stream2hex(str, s, true);
    return s;
}

// wrapper for convert hex to string
string hex2string(const string hex_str) {
    string s;
    hex2stream(hex_str, s);
    return s;
}

int main() {
    string result, ans;
    result = des_encrypt("12341234", "12341234");
    if (string2hex(result) == "0D05989E6F2605FA") printf("Encrypt is true\n");
    ans = des_decrypt(hex2string("0D05989E6F2605FA"), "12341234");
    if (ans == "12341234") printf("Decrypt is true\n");
    if (string2hex(sha1_digest("test")) == "A94A8FE5CCB19BA61C4C0873D391E987982FBBD3") printf("SHA_1 is true\n");
    return 0;
}