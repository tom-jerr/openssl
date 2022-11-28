#include <bits/stdc++.h>

#include <openssl/bn.h>

using namespace std;

// 该函数用于进行大数模指数运算
//  参数:
//      string类型, 求解a^e mod m, 表示为10进制数字符串
//  返回值:
//      string类型, 返回计算的结果, 表示为10进制数字符串
string mod_exp(string a, string e, string m) {
	//BIGNUM* a1; BIGNUM* e1; BIGNUM* m1;BIGNUM* ans; BIGNUM* temp;
	//BN_CTX *ctx=NULL;
	//ctx=BN_CTX_new();
	//BN_CTX_start(ctx);

	//a1=BN_new();e1=BN_new();m1=BN_new();ans=BN_new();temp=BN_new();
	//BN_dec2bn(&a1, a.c_str()); BN_dec2bn(&e1, e.c_str()); BN_dec2bn(&m1, m.c_str());
	//
	//BN_one(ans);
	//while (!BN_is_zero(e1)){
	//	BN_mul(a1,a1,a1,ctx); BN_nnmod(a1,a1,m1,ctx);
	//	BN_copy(temp, e1);
	//	int length = BN_num_bits(temp);
	//	BN_rshift(temp,temp,length-1);
	//	if (BN_is_one(temp)){
	//		BN_mul(ans,a1,ans,ctx);
	//		BN_nnmod(ans,ans,m1,ctx);
	//	}
	//	BN_rshift1(e1,e1);
	//}
	////实现BN的快速幂乘法
	//char* result = BN_bn2dec(ans);
	//BN_CTX_end(ctx);
	//BN_CTX_free(ctx);
	//BN_free(a1);BN_free(e1);BN_free(m1);BN_free(ans);BN_free(temp);
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

int main(){
	/*string result = mod_exp("82019154470699086128524248488673846867876336512717", "82019154470699",
                    "8201915447069908612852424848867384686787636512717");*/
	string result = mod_exp("2","5","5");
	printf("%s\n", result.c_str());
	if (result == "2"){
		printf("This is True\n");
		return 0;
	}
	
	printf("This is not True\n");
	return 0;
}