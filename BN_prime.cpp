#include <bits/stdc++.h>
#include <openssl/bn.h>

int main() {
	char a[20] = {"97"};
	BIGNUM* b = BN_new();
	BN_CTX* c = BN_CTX_new();
	BN_CTX_start(c);
	/*BN_dec2bn(&b, (const char*)a);*/

	//����������������5���������Բ��ӣ���2����������ָ�����ɶ���bit������
	BN_generate_prime(b,1024,NULL,NULL,NULL,NULL,NULL);

	//�ж��Ƿ�Ϊ������������2������Ϊ���Դ���
	int flag = BN_is_prime(b, 512, NULL, c, NULL);
	printf("%d", flag);
	return 0;
}