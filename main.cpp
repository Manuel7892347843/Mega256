#include <iostream>

#include "Mega256.h"

int main() {
    Mega256 num(0XFFFFFFFFFFFFFFFFULL);
    Mega256 num2(0XFFFFFFFFFFFFFFFFULL);

    std::cout << "Num 1: " ; num.print();
    std::cout << "Num 2: " ; num2.print();

    Mega256 sum = num + num2;
    std::cout << "Sum  : " ; sum.print();
    std::string ris_dec = sum.toDecString();
    std::cout << "Dec  : " << ris_dec << std::endl;

    Mega256 max = Mega256::MAX_VALUE;
    std::cout << "MAX  : " << max.toDecString() << std::endl;

    Mega256 min = Mega256::MIN_VALUE;
    std::cout << "MIN  : " << min.toDecString() << std::endl;
    return 0;
}
