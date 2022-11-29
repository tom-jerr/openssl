#include <bits/stdc++.h>
using namespace std;

void swap(unsigned char& a, unsigned char& b) {
	unsigned char tmp = a;
	a = b;
	b = tmp;
}

/// <summary>
///		KSA + PRGA生成RC4密钥流
/// </summary>
/// <param name="key">用户输入的密钥</param>
/// <param name="data">需要加密的数据</param>
/// <param name="k">生成密钥流</param>
/// <returns>数据长度</returns>
int RC4_KSA(unsigned char key[], unsigned char data[], unsigned char k[]) {
	unsigned char S[256];
	unsigned char T[256];

	int keylen, datalen, i, j;
	for (i = 0;; i++) {
		if (key[i] == '\0') {
			keylen = i;
			break;
		}
	}
	for (i = 0;; i++) {
		if (data[i] == '\0') {
			datalen = i;
			break;
		}
	}

	for (i = 0; i < 256; i++) S[i] = i;
	if (keylen > 256) {
		for (i = 0; i < 256; i++) T[i] = key[i];
	}
	else {
		j = 0;
		for (i = 0; i < 256; i++, j++) {
			T[i] = key[j % keylen];
		}
	}

	i = 0; j = 0;
	for (i = 0; i < 256; i++) {
		j = (j + S[i] + T[i]) % 256;
		swap(S[i], S[j]);
	}

	//PRGA伪随机生成算法，生成密钥流
	int times = datalen; int m = 0; int tmp;
	while (times--){
		i = (i + 1) % 256;
		j = (j + S[i]) % 256;
		swap(S[i], S[j]);
		tmp = (S[i] + S[j]) % 256;
		k[m++] = S[tmp];
	}
	return datalen;
}
/// <summary>
///		RC4异或加密明文
/// </summary>
/// <param name="data">数据</param>
/// <param name="k">生成的密钥</param>
/// <param name="datalen">数据长度</param>
/// <returns>生成密文的字符串</returns>
string RC4_encrypt(unsigned char data[], unsigned char k[], int datalen) {
	for (int i = 0; i < datalen; i++) {
		data[i] ^= k[i];
	}
	string res((char*)data, datalen);
	return res;
}
/// <summary>
///		RC4异或解密明文
/// </summary>
/// <param name="data">数据</param>
/// <param name="k">生成的密钥</param>
/// <param name="datalen">数据长度</param>
/// <returns>解密明文的字符串</returns>
string RC4_decrypt(string cipher, unsigned char k[], int datalen) {
	char* data = (char*)cipher.c_str();
	for (int i = 0; i < datalen; i++) {
		data[i] ^= k[i];
	}
	string res((char*)data, datalen);
	return res;
}

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
	unsigned char key[] = { "hello world" };
	unsigned char data[] = { "北京时间十八点实施轰炸!北京时间十八点实施轰炸!北京时间十八点实施轰炸!" };
	unsigned char k[1024];
	int datalen = RC4_KSA(key, data, k);
	string cipher = RC4_encrypt(data, k, datalen);
	string plain = RC4_decrypt(cipher, k, datalen);
	printf("The cipher is:\n");
	printf("%s\n", string2hex(cipher).c_str());

	printf("The plain is:\n");
	printf("%s\n", plain.c_str());
}