#include <bits/stdc++.h>
#include <openssl/bn.h>
using namespace std;
std::string miller_rabin_prime_test(unsigned int n, unsigned int a) {
	if (n < 2 || n % 2 == 0)return "not_prime";
	if (a < 2 || a >= n - 1)return "error";
	int k = 0, q = n - 1;
	unsigned long long temp1 = a;

	while (!(q & 1))
	{
		k++;
		q >>= 1;
	}
	for (int i = 1; i < q; i++) {
		temp1 = (unsigned long long) (temp1 * a) % n;
	}
	if (temp1 == 1 || temp1 == n - 1) return "uncertain";
	for (int i = 1; i < k && temp1 != (n - 1); i++) {
		temp1 = (unsigned long long)(temp1 * temp1) % n;
		if (temp1 == n - 1) return "uncertain";
	}
	return "not_prime";
}

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
	printf("%d\n", flag);
	string result = miller_rabin_prime_test(97, 3);
	printf("%s\n", result.c_str());
	return 0;
}