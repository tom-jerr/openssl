# openssl实验中的诸多问题
## 1. openssl中使用String和unsigned char\* 以及对const char\*的相互转换
1. 将string转为字符数组：
	string.c_str():将string转为const char\*，末尾有'\0';
	string.data():将string转为const char\*，末尾无字符
2. const char\* 与 unsigned char\* 以及 unsigned const char\*均需要强制转换；同时strlen函数只能对const char\*进行计数操作

***

## 2. BN库中生成及判断函数（典型）

`BN_generate_prime()`指定生成多少bit的素数；

`BN_is_prime()`判断当前传入大数是否为素数；返回0说明有0.25几率不是大数；

~~~c
#include <bits/stdc++.h>
#include <openssl/bn.h>

int main() {
	char a[20] = {"97"};
	BIGNUM* b = BN_new();
	BN_CTX* c = BN_CTX_new();
	BN_CTX_start(c);
	/*//将字符串传入数字转为大数形式
	BN_dec2bn(&b, (const char*)a);*/

	//生成素数函数，后5个参数可以不加；第2个参数用于指定生成多少bit的素数
	BN_generate_prime(b,1024,NULL,NULL,NULL,NULL,NULL);

	//判断是否为素数函数，第2个参数为测试次数
	int flag = BN_is_prime(b, 512, NULL, c, NULL);
	printf("%d", flag);
	return 0;
}
~~~

***

## 3. 进制转换一般解决方案

`TODO`未解决负数转换进制的问题

~~~c
#include <iostream>
#include <string>
using namespace std;
unsigned int binary_to_decimal(std::string input) {
    unsigned int res = 0;
    for (int i = 0; i < input.length(); i++) {
        if (input[i] == '1') {
            res <<= 1;
            res += 1;
        }
        else if (input[i] == '0') res <<= 1;
        else break;
    }
    return res;
}

std::string decimal_to_binary(unsigned int input) {
    string binary; int flag = 0;
    if (input < 0) flag = 1;

    while (input > 0) {
        if (input % 2 == 1) {
            binary.insert(0, "1");
        }
        else {
            binary.insert(0, "0");
        }
        input = input >> 1; // 取除以2后的数
    }
    if (flag == 1) {
        //TODO 对负数进行进制转换
        return binary;
    }
    return binary;
}

int main() {
    unsigned int res = binary_to_decimal("ihNIbONkBUBD8918918");
    unsigned int res1 = binary_to_decimal("00000000000000001111111111111100000000000000010101010000000000000001000000");
    //356515904
    printf("%u\n", res1);
    printf("%u\n", res);
}
~~~

***

## 4. BN的指数幂求解及模逆求解

1. 使用自带的库函数：`BN_mod_exp()`和`BN_mod_inverse()`

   ~~~c
   string mod_exp(string a, string e, string m) {
   	/*
   		BN大数库中标准计算mod_exp的函数
   	*/
   
   	BIGNUM* a1; BIGNUM* e1; BIGNUM* m1; BIGNUM* res;
   	int flag=0;
   	char* result=NULL;
   	BN_CTX *ctx=NULL;
   	ctx=BN_CTX_new();
   	BN_CTX_start(ctx);
   	a1=BN_new();e1=BN_new();m1=BN_new();res=BN_new();
   	if(BN_dec2bn(&a1,a.c_str())&&BN_dec2bn(&e1,e.c_str())&&BN_dec2bn(&m1,m.c_str())){
   
   		flag=BN_mod_exp_simple(res,a1,e1,m1,ctx);
   	}
   	if (flag==1){
   		result=BN_bn2dec(res);
   	}
   	BN_CTX_end(ctx);
   	BN_CTX_free(ctx);
   	BN_free(a1);BN_free(e1);BN_free(m1);BN_free(res); 
   	return result;	
   }
   
   string mod_inverse(string a, string m) {
   	BIGNUM* a1; BIGNUM* m1; BIGNUM* r1 ;
   	char* res = NULL;
   	a1 = BN_new(); m1 = BN_new(); r1 = BN_new();
   	BN_CTX* ctx = NULL;
   	ctx = BN_CTX_new();
   	BN_CTX_start(ctx);
   	BN_dec2bn(&a1, a.c_str()); BN_dec2bn(&m1, m.c_str());
   	BN_mod_inverse(r1, a1, m1, ctx);
   	res = BN_bn2dec(r1);
   	BN_CTX_end(ctx);
   	BN_CTX_free(ctx);
   	BN_free(a1); BN_free(m1); BN_free(r1); 
   	return res;
   }
   ~~~

   

2. 使用BN库中基本运算函数实现；

   ~~~c
   string mod_exp(string a, string e, string m) {
   	/*
   		使用大数基本运算实现mod_exp运算；解决问题！！！
   	*/
   	
   	BIGNUM* a1; BIGNUM* a2; BIGNUM* e1; BIGNUM* m1; BIGNUM* ans; BIGNUM* temp;
   	BN_CTX *ctx=NULL;
   	ctx=BN_CTX_new();
   	BN_CTX_start(ctx);
   
   	a1 = BN_new(); a2 = BN_new(); e1 = BN_new(); m1 = BN_new(); ans = BN_new(); temp = BN_new();
   	BN_dec2bn(&a1, a.c_str()); BN_dec2bn(&a2, a.c_str()); BN_dec2bn(&e1, e.c_str()); BN_dec2bn(&m1, m.c_str());
   
   	BN_one(ans);
   	unsigned long long length = (unsigned long long)BN_num_bits(e1); int flag = 0;
   	while ((length - 1) > 0){
   		BN_mul(ans,ans,ans,ctx); 
   		BN_nnmod(ans,ans,m1,ctx);
   		BN_copy(temp, e1);
   		int length1 = BN_num_bits(temp);
   		
   		if (length1 != length - 1 && flag == 1) {
   			length = length - 1;
   			for (unsigned long long i = (unsigned long long)length1 + 1 ; i <= length  ; i++) {
   				BN_set_bit(e1, (int)i);
   			}
   			BN_copy(temp, e1);
   		}
   		else {
   			length = length1;
   			flag = 1;
   		}
   		BN_rshift(temp,temp,length - 1);
   		BN_mask_bits(temp, 1);
   		if (BN_is_one(temp) && !BN_is_bit_set(temp, 1)){
   			BN_mul(ans,a1,ans,ctx);
   			BN_nnmod(ans,ans,m1,ctx);
   		}
   		BN_mask_bits(e1,length - 1);
   	}
   	
   	char* result = BN_bn2dec(ans);
   	BN_CTX_end(ctx);
   	BN_CTX_free(ctx);
   	BN_free(a1);BN_free(e1);BN_free(m1);BN_free(ans);BN_free(temp);
   	return result;
   	/*
   		纯暴力方式，大数运算耗时过长
   	*/
   	/*BIGNUM* a1; BIGNUM* a2; BIGNUM* e1; BIGNUM* m1; BIGNUM* ans; BIGNUM* temp;
   	BN_CTX *ctx=NULL;
   	ctx=BN_CTX_new();
   	BN_CTX_start(ctx);
   	a1 = BN_new(); a2 = BN_new(); e1 = BN_new(); m1 = BN_new(); ans = BN_new(); temp = BN_new();
   	BN_dec2bn(&a1, a.c_str()); BN_dec2bn(&a2, a.c_str()); BN_dec2bn(&e1, e.c_str()); BN_dec2bn(&m1, m.c_str());
   	BN_one(temp);
   	while (!BN_is_one(e1)) {
   		BN_mul(a2,a2,a1,ctx);
   		BN_mod(a2,a2,m1,ctx);
   		BN_sub(e1, e1, temp);
   	}
   
   	char* result = BN_bn2dec(a2);
   	BN_CTX_end(ctx);
   	BN_CTX_free(ctx);
   	BN_free(a1);BN_free(e1);BN_free(m1);BN_free(ans);BN_free(temp);
   	return result;*/
   	
   }
   
   BIGNUM* gcd_exp(BIGNUM* a, BIGNUM* b, BIGNUM* x, BIGNUM* y) {
   	if (BN_is_zero(b)) {
   		BN_zero(y); BN_one(x);
   		return a;
   	}
   	BN_CTX* ctx1=NULL; ctx1 = BN_CTX_new();
   	BN_CTX_start(ctx1);
   
   	BIGNUM* mod_; BIGNUM* dv; BIGNUM* rem; BIGNUM* mult;
   	mod_ = BN_new(); dv = BN_new(); rem = BN_new(); mult = BN_new();
   	BN_nnmod(mod_, a, b, ctx1);
   	BIGNUM* temp = BN_new();
   	temp = gcd_exp(b, mod_, y, x);
   	BN_div(dv, rem, a, b, ctx1);
   	BN_mul(mult, dv, x, ctx1);
   	BN_sub(y, y, mult);
   	BN_CTX_end(ctx1);
   	BN_CTX_free(ctx1);
   	BN_free(mod_); BN_free(dv); BN_free(rem); BN_free(mult); 
   	return temp;
   }
   
   string mod_inverse(string a, string m) {
   	BIGNUM* a1; BIGNUM* m1; BIGNUM* x; BIGNUM* y; BIGNUM* r1 = NULL;
   	char* res = NULL;
   	a1 = BN_new(); m1 = BN_new(); x = BN_new(); y = BN_new(); 
   	/*r1 = BN_new();*/
   	BN_CTX* ctx = NULL;
   	ctx = BN_CTX_new();
   	BN_CTX_start(ctx);
   	BN_dec2bn(&a1, a.c_str()); BN_dec2bn(&m1, m.c_str());
   	r1 = gcd_exp(a1, m1, x, y);
   
   	res = BN_bn2dec(x);
   	BN_CTX_end(ctx);
   	BN_CTX_free(ctx);
   	BN_free(a1); BN_free(m1); BN_free(x); BN_free(y);
   	return res;
   }
   ~~~

***

## 5. DES加密的各种方式

`ECB`模式每次只能加密8个字节长度的数据

~~~C
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

void convert_string_to_des_block(string str, DES_cblock& output) {
    for (int i = 0; i < 8; ++i) {
        output[i] = str[i];
    }
}
~~~

***

## 6. BN大数库实现RSA加解密

~~~C
void main()
{
	BIGNUM* p = BN_new();    //大素数p
	BIGNUM* q = BN_new();  //大素数q
	BIGNUM* n = BN_new();    //n=p*q
	BIGNUM* fn = BN_new();   //fn=(p-1)*(q-1)
	BIGNUM* d = BN_new();    //d=e-1 mod (n)
	BIGNUM* e = BN_new();   //整数e，1<e<fn且gcd(fn, e)=1
	BIGNUM* r = BN_new();
	BIGNUM* M = BN_new();    //明文
	BIGNUM* C = BN_new();    //密文
	BIGNUM* one = BN_new();  //将one设置为1
	BN_one(one);
	int length;
	char str[100] = "0";
	char* show;
	int bits = 512;     //512bits
	int i, j, t;
	int sum = 0;
	printf("请输入要加密的的明文：");
	scanf_s("%s", str, sizeof(str));
	length = strlen(str);                //记录明文的长度
	BN_bin2bn((const unsigned char*)str, length, M);            //将明文以十六进制转换为大数
	printf("明文:%s\n", str);
	BN_generate_prime(p, bits, 1, NULL, NULL, NULL, NULL);       //生成512bits的大素数p
	BN_generate_prime(q, bits, 1, NULL, NULL, NULL, NULL);     //生成512bits的大素数q
	BN_CTX* ctx = BN_CTX_new();         //上下文结构		
	BN_mul(n, p, q, ctx);            //n=p*q
	BN_sub(p, p, one);
	BN_sub(q, q, one);
	BN_mul(fn, p, q, ctx);           //fn=(p-1)*(q-1)
	do
	{
		BN_rand_range(e, fn); //产生的0 < e < fn的随机数
		BN_gcd(r, e, fn, ctx); //r = e, fn最大公约数
	} while (!BN_is_one(r)); //判断r是否等于1
	BN_mod_inverse(d, e, fn, ctx);    //模逆运算
	//公钥加密 C = M^e mod n
	BN_mod_exp_simple(C, M, e, n, ctx);
	show = BN_bn2hex(C);     //将密文转换为十六进制
	printf("密文:%s\n", show);
	//私钥解密 M = C^d mod n。
	BN_mod_exp_simple(M, C, d, n, ctx);
	show = BN_bn2hex(M);
	//printf("%x",show);//测试
	printf("\n解密后的明文:%s", show);  //将解密后的明文转换为十六进制
	printf("\n解密后的明文:");

	for (i = 0; i < length; i++)  //将解密后的明文转化为对应的ASCII的字符
	{
		j = *(show + 2 * i);   //每两位为一个整体
		t = *(show + 2 * i + 1);
		j = zhuanhuan(j);
		t = zhuanhuan(t);
		sum = j * 16 + t;
		printf("%c", sum); //将ASCII的字符输出
	}
	//释放结构
	BN_CTX_free(ctx);
	BN_free(p);
	BN_free(q);
	BN_free(n);
	BN_free(fn);
	BN_free(d);
	BN_free(e);
	BN_free(r);
	BN_free(M);
	BN_free(C);
}
~~~

***

## 7. RC4加解密

1. 按流程实现RC4

   ~~~C
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
   ~~~

   

2. openssl库中直接调用函数

   ~~~C
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
   ~~~

   












