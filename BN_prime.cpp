#include <bits/stdc++.h>
#include <openssl/bn.h>

int main() {
	char a[20] = {"97"};
	BIGNUM* b = BN_new();
	BN_CTX* c = BN_CTX_new();
	BN_CTX_start(c);
	/*BN_dec2bn(&b, (const char*)a);*/

	//生成素数函数，后5个参数可以不加；第2个参数用于指定生成多少bit的素数
	BN_generate_prime(b,1024,NULL,NULL,NULL,NULL,NULL);

	//判断是否为素数函数，第2个参数为测试次数
	int flag = BN_is_prime(b, 512, NULL, c, NULL);
	printf("%d", flag);
	return 0;
}