#include <iostream>
#include "Mega256.h"

int main() {
    Mega256 num1(0XFFFFFFFFFFFFFFFFULL);
    Mega256 num2(0XFFFFFFFFFFFFFFFFULL);
    Mega256 sum = num1 + num2;
    std::cout << "Hex Sum: " << sum.toString() << std::endl;
    std::cout << "Dec Sum: " << sum.toDecString() << std::endl;

    Mega256 pos_val("1000000000000000000000000");
    Mega256 neg_val("-500000000000000000000000");
    Mega256 mixed_sum = pos_val + neg_val;
    std::cout << "Mixed Sign Sum: " << mixed_sum.toDecString() << std::endl;

    Mega256 small(100);
    Mega256 large(500);
    Mega256 negative_res = small - large;
    std::cout << "Negative Result: " << negative_res.toDecString() << std::endl;

    Mega256 factor1("12345678901234567890");
    Mega256 factor2("-98765432109876543210");
    Mega256 product = factor1 * factor2;
    std::cout << "Product: " << product.toDecString() << std::endl;

    Mega256 dividend("500000000000000000000000000000");
    Mega256 divisor("2500000000000000000000000");
    Mega256 quotient = dividend / divisor;
    std::cout << "Quotient: " << quotient.toDecString() << std::endl;

    Mega256 max = Mega256::MAX_VALUE;
    Mega256 min = Mega256::MIN_VALUE;
    std::cout << "MAX Value: " << max.toDecString() << std::endl;
    std::cout << "MIN Value: " << min.toDecString() << std::endl;

    return 0;
}
