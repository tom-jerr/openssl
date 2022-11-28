#include <bits/stdc++.h>
#include <openssl/bn.h>
using namespace std;

int zhuanhuan(int a)//将十六进制转换为十进制
{
	if (a < 60)
	{
		a = a - '0';
	}
	if (a > 64)
	{
		a = a - 'A' + 10;
	}
	return a;
}
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
