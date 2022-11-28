#include <bits/stdc++.h>

#include <openssl/bn.h>

using namespace std;

std::string miller_rabin_prime_test(unsigned int n, unsigned int a) {
	if (n < 2 || n % 2 == 0)return "not_prime";
	if (a < 2 || a >= n - 1)return "error";
	int k = 0; unsigned int q = n - 1;
	unsigned long long temp1 = a % n;
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
	// temp1=Quick_pow(a,q,n);
	// int count=0;
	// 	for(int i=0;i<k;i++)
	// 	{
	// 		temp2=Quick_mul(temp1,temp1,n);
	// 		if(temp3!=1 && temp1!=n-1)count++;
	// 		temp1=temp2;
	// 	}
	// 	if(temp2!=1)return "not_prime";	//a^(2^k)-1=0;
	// 	if(count<k-1)return "not_prime";
	return "not_prime";
}

bool prime_test(unsigned int a)
{
	if (a == 0) { return false; }
	if (a == 1) { return false; }
	if (a == 2) { return true; }
	double n_sqr = sqrt(a);
	unsigned int n_sqr_ceil = (unsigned int)(n_sqr)+1;
	std::vector<int> flag(70000);
	unsigned int i, j, detect;
	for (i = 2; i <= n_sqr_ceil; i++) {
		j = i * i;
		while (j <= n_sqr_ceil) {
			flag[j] = 1;
			j += i;
		}
	}
	i = 2;
	while (flag[i] == 0 && i <= n_sqr_ceil) {
		detect = a % i;
		if (detect == 0) { return false; }
		i++;
	}
	return true;
}

int main() {
	string res = miller_rabin_prime_test(221, 200);
	printf("%s\n", res.c_str());
	if (res == "uncertain") printf("miller_rabin_prime_test is true\n");
	else printf("This is not true\n");
	if (prime_test(4294967291) == true) printf("prime_test is true");
	return 0;
}