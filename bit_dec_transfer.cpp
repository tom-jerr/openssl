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