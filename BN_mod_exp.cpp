#include <bits/stdc++.h>

#include <openssl/bn.h>

using namespace std;

// 该函数用于进行大数模指数运算
//  参数:
//      string类型, 求解a^e mod m, 表示为10进制数字符串
//  返回值:
//      string类型, 返回计算的结果, 表示为10进制数字符串
string mod_exp(string a, string e, string m) {
	/*
		使用大数基本运算实现mod_exp运算
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
			BN_lshift(e1, e1, length - length1);
			BN_set_bit(e1, length1 - 1);
			for (unsigned long long i = length1 ; i <= length - 1 ; i++) {
				BN_clear_bit(e1, (int)i);
			}
			BN_copy(temp, e1);
		}
		else {
			length = length1;
			flag = 1;
		}
		BN_rshift(temp,temp,length - 1);
		BN_mask_bits(temp, 1);
		if (BN_is_one(temp)){
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
		BN大数库中标准计算mod_exp的函数
	*/

	/*BIGNUM* a1; BIGNUM* e1; BIGNUM* m1; BIGNUM* res;
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
	return result;*/

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
	return res;
	BN_CTX_end(ctx);
	BN_CTX_free(ctx);
	BN_free(a1); BN_free(m1); BN_free(r1); BN_free(x); BN_free(y);
}

int main(){
	/*string result1 = mod_exp("82019154470699086128524248488673846867876336512717", "82019154470699",
	                   "8201915447069908612852424848867384686787636512717");
	printf("%s\n", result1.c_str());*/
	string result = mod_exp("23","2","100");
	printf("%s\n", result.c_str());
	
	if (result == "29"){
		printf("This is True\n");
		return 0;
	}
	
	printf("This is not True\n");
	/*string result = mod_inverse("5", "14");
	if (result == "3") {
		printf("This is True\n");
		return 0;
	}*/
	return 0;
}