#include <bits/stdc++.h>

#include <openssl/bn.h>

using namespace std;

// 该函数用于进行大数模指数运算
//  参数:
//      string类型, 求解a^e mod m, 表示为10进制数字符串
//  返回值:
//      string类型, 返回计算的结果, 表示为10进制数字符串
string mod_exp(string a, string e, string m) {
	
	//BN_CTX *ctx=NULL;
	//ctx=BN_CTX_new();
	//BN_CTX_start(ctx);
	//
	//BIGNUM* ans;BIGNUM* temp;
	//
	//a1=BN_new();e1=BN_new();m1=BN_new();ans=BN_new();temp=BN_new();
	//BN_dec2bn(&a1, a.c_str()); BN_dec2bn(&e1, e.c_str()); BN_dec2bn(&m1, m.c_str());
	//
	//BN_one(ans);
	//while (!BN_is_zero(e1)){
	//	BN_mul(a1,a1,a1,ctx); BN_mod(a1,a1,m1,ctx);
	//	BN_copy(temp, e1);
	//	int length = BN_num_bits(temp);
	//	BN_rshift(temp,temp,length);
	//	if (BN_is_one(temp)){
	//		BN_mul(ans,a1,ans,ctx);
	//		BN_mod(ans,ans,m1,ctx);
	//	}
	//	BN_rshift1(e1,e1);
	//}
	////实现BN的快速幂乘法
	//char* res = BN_bn2dec(ans);
	//BN_CTX_end(ctx);
	//BN_CTX_free(ctx);
	//BN_free(a1);BN_free(e1);BN_free(m1);BN_free(ans);BN_free(temp);
	BIGNUM* a1; BIGNUM* e1; BIGNUM* m1; BIGNUM* res;
	const char* a_=a.data();
	const char* e_=e.data();
	const char* m_=m.data();
	int flag=0;
	string result="";
	BN_CTX *ctx=NULL;
	ctx=BN_CTX_new();
	BN_CTX_start(ctx);
	a1=BN_new();e1=BN_new();m1=BN_new();res=BN_new();
	if(BN_dec2bn(&a1,a_)&&BN_dec2bn(&e1,e_)&&BN_dec2bn(&m1,m_)){

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
	string result = mod_exp("82019154470699086128524248488673846867876336512717", "82019154470699",
                    "8201915447069908612852424848867384686787636512717");
	printf("%s\n", result.c_str());
	if (result == "742515522565090435181758308372402438050496506811"){
		printf("This is True\n");
		return 0;
	}
	
	printf("This is not True\n");
	return 0;
}