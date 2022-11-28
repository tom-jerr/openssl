#include <bits/stdc++.h>
#include <openssl/bn.h>
using namespace std;

int zhuanhuan(int a)//��ʮ������ת��Ϊʮ����
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
	BIGNUM* p = BN_new();    //������p
	BIGNUM* q = BN_new();  //������q
	BIGNUM* n = BN_new();    //n=p*q
	BIGNUM* fn = BN_new();   //fn=(p-1)*(q-1)
	BIGNUM* d = BN_new();    //d=e-1 mod (n)
	BIGNUM* e = BN_new();   //����e��1<e<fn��gcd(fn, e)=1
	BIGNUM* r = BN_new();
	BIGNUM* M = BN_new();    //����
	BIGNUM* C = BN_new();    //����
	BIGNUM* one = BN_new();  //��one����Ϊ1
	BN_one(one);
	int length;
	char str[100] = "0";
	char* show;
	int bits = 512;     //512bits
	int i, j, t;
	int sum = 0;
	printf("������Ҫ���ܵĵ����ģ�");
	scanf_s("%s", str, sizeof(str));
	length = strlen(str);                //��¼���ĵĳ���
	BN_bin2bn((const unsigned char*)str, length, M);            //��������ʮ������ת��Ϊ����
	printf("����:%s\n", str);
	BN_generate_prime(p, bits, 1, NULL, NULL, NULL, NULL);       //����512bits�Ĵ�����p
	BN_generate_prime(q, bits, 1, NULL, NULL, NULL, NULL);     //����512bits�Ĵ�����q
	BN_CTX* ctx = BN_CTX_new();         //�����Ľṹ		
	BN_mul(n, p, q, ctx);            //n=p*q
	BN_sub(p, p, one);
	BN_sub(q, q, one);
	BN_mul(fn, p, q, ctx);           //fn=(p-1)*(q-1)
	do
	{
		BN_rand_range(e, fn); //������0 < e < fn�������
		BN_gcd(r, e, fn, ctx); //r = e, fn���Լ��
	} while (!BN_is_one(r)); //�ж�r�Ƿ����1
	BN_mod_inverse(d, e, fn, ctx);    //ģ������
	//��Կ���� C = M^e mod n
	BN_mod_exp_simple(C, M, e, n, ctx);
	show = BN_bn2hex(C);     //������ת��Ϊʮ������
	printf("����:%s\n", show);
	//˽Կ���� M = C^d mod n��
	BN_mod_exp_simple(M, C, d, n, ctx);
	show = BN_bn2hex(M);
	//printf("%x",show);//����
	printf("\n���ܺ������:%s", show);  //�����ܺ������ת��Ϊʮ������
	printf("\n���ܺ������:");

	for (i = 0; i < length; i++)  //�����ܺ������ת��Ϊ��Ӧ��ASCII���ַ�
	{
		j = *(show + 2 * i);   //ÿ��λΪһ������
		t = *(show + 2 * i + 1);
		j = zhuanhuan(j);
		t = zhuanhuan(t);
		sum = j * 16 + t;
		printf("%c", sum); //��ASCII���ַ����
	}
	//�ͷŽṹ
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
