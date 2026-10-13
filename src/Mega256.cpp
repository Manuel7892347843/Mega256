#include "Mega256.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>

const Mega256 Mega256::MAX_VALUE = [] {
    Mega256 m;
    m.data.fill(0xFFFFFFFFFFFFFFFFULL);
    m.is_negative = false;
    return m;
}();

const Mega256 Mega256::MIN_VALUE = [] {
    Mega256 m;
    m.data.fill(0xFFFFFFFFFFFFFFFFULL);
    m.is_negative = true;
    return m;
}();

Mega256::Mega256() {
    data.fill(0);
    is_negative = false;
}

Mega256::Mega256(int64_t value) {
    data.fill(0);
    if (value < 0) {
        is_negative = true;
        data[0] = static_cast<uint64_t>(-value);
    } else {
        is_negative = false;
        data[0] = static_cast<uint64_t>(value);
    }
}

Mega256::Mega256(uint64_t value) {
    data.fill(0);
    data[0] = value;
    is_negative = false;
}

Mega256::Mega256(const std::string &str) {
    data.fill(0);
    is_negative = false;
    if (str.empty()) return;

    size_t start_idx = 0;
    if (str[0] == '-') {
        is_negative = true;
        start_idx = 1;
    } else if (str[0] == '+') {
        start_idx = 1;
    }

    if (start_idx == str.length()) {
        throw std::invalid_argument("This string contains non-digit characters");
    }

    for (size_t idx = start_idx; idx < str.length(); ++idx) {
        char c = str[idx];
        if (c < '0' || c > '9')
            throw std::invalid_argument("This string contains non-digit characters");
        uint64_t digit = c - '0';
        uint64_t carry = digit;
        for (size_t i = 0; i < 4; ++i) {
            __uint128_t product = (static_cast<__uint128_t>(data[i]) * 10) + carry;
            data[i] = static_cast<uint64_t>(product);
            carry = static_cast<uint64_t>(product >> 64);
        }
    }
    if (data[0] == 0 && data[1] == 0 && data[2] == 0 && data[3] == 0) {
        is_negative = false;
    }
}

Mega256 Mega256::add_absolute(const Mega256& other) const {
    Mega256 result;
    uint64_t carry = 0;
    for (size_t i = 0; i < 4; ++i) {
        __uint128_t sum = static_cast<__uint128_t>(this->data[i]) + static_cast<__uint128_t>(other.data[i]) + carry;
        result.data[i] = static_cast<uint64_t>(sum);
        carry = static_cast<uint64_t>(sum >> 64);
    }
    return result;
}

Mega256 Mega256::sub_absolute(const Mega256& other) const {
    Mega256 result;
    uint64_t borrow = 0;
    for (size_t i = 0; i < 4; ++i) {
        __uint128_t diff = static_cast<__uint128_t>(this->data[i]) - static_cast<__uint128_t>(other.data[i]) - borrow;
        result.data[i] = static_cast<uint64_t>(diff);
        if (this->data[i] < other.data[i] + borrow) {
            borrow = 1;
        } else {
            borrow = 0;
        }
    }
    return result;
}

bool Mega256::lt_absolute(const Mega256& other) const {
    for (int i = 3; i >= 0; --i) {
        if (this->data[i] < other.data[i]) return true;
        if (this->data[i] > other.data[i]) return false;
    }
    return false;
}

Mega256 Mega256::operator+(const Mega256 &other) const {
    Mega256 result;
    if (this->is_negative == other.is_negative) {
        result = this->add_absolute(other);
        result.is_negative = this->is_negative;
    } else {
        if (this->lt_absolute(other)) {
            result = other.sub_absolute(*this);
            result.is_negative = other.is_negative;
        } else {
            result = this->sub_absolute(other);
            result.is_negative = this->is_negative;
        }
    }
    if (result.data[0] == 0 && result.data[1] == 0 && result.data[2] == 0 && result.data[3] == 0) {
        result.is_negative = false;
    }
    return result;
}

Mega256 Mega256::operator-(const Mega256 &other) const {
    Mega256 temp = other;
    temp.is_negative = !other.is_negative;
    if (temp.data[0] == 0 && temp.data[1] == 0 && temp.data[2] == 0 && temp.data[3] == 0) {
        temp.is_negative = false;
    }
    return *this + temp;
}

Mega256 Mega256::operator*(const Mega256 &other) const {
    Mega256 result;
    for (size_t i = 0; i < 4; ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j + i < 4; ++j) {
            __uint128_t product = static_cast<__uint128_t>(this->data[i]) * static_cast<__uint128_t>(other.data[j]) + result.data[i + j] + carry;
            result.data[i + j] = static_cast<uint64_t>(product);
            carry = static_cast<uint64_t>(product >> 64);
        }
    }
    result.is_negative = this->is_negative != other.is_negative;
    if (result.data[0] == 0 && result.data[1] == 0 && result.data[2] == 0 && result.data[3] == 0) {
        result.is_negative = false;
    }
    return result;
}

void Mega256::shiftLeftOne() {
    uint64_t carry = 0;
    for (size_t i = 0; i < 4; ++i) {
        uint64_t next_carry = (data[i] & 0x8000000000000000ULL) ? 1 : 0;
        data[i] = (data[i] << 1) | carry;
        carry = next_carry;
    }
}

Mega256 Mega256::operator/(const Mega256 &other) const {
    if (other.data[0] == 0 && other.data[1] == 0 && other.data[2] == 0 && other.data[3] == 0) {
        throw std::invalid_argument("Division by zero");
    }
    Mega256 quotient;
    Mega256 remainder;
    Mega256 abs_other = other;
    abs_other.is_negative = false;

    for (int i = 255; i >= 0; --i) {
        remainder.shiftLeftOne();
        if (this->data[i / 64] & (1ULL << (i % 64))) {
            remainder.data[0] |= 1ULL;
        }
        if (remainder >= abs_other) {
            remainder -= abs_other;
            quotient.data[i / 64] |= (1ULL << (i % 64));
        }
    }
    quotient.is_negative = this->is_negative != other.is_negative;
    if (quotient.data[0] == 0 && quotient.data[1] == 0 && quotient.data[2] == 0 && quotient.data[3] == 0) {
        quotient.is_negative = false;
    }
    return quotient;
}

Mega256 Mega256::operator%(const Mega256 &other) const {
    if (other.data[0] == 0 && other.data[1] == 0 && other.data[2] == 0 && other.data[3] == 0) {
        throw std::invalid_argument("Division by zero");
    }
    Mega256 remainder;
    Mega256 abs_other = other;
    abs_other.is_negative = false;

    for (int i = 255; i >= 0; --i) {
        remainder.shiftLeftOne();
        if (this->data[i / 64] & (1ULL << (i % 64))) {
            remainder.data[0] |= 1ULL;
        }
        if (remainder >= abs_other) {
            remainder -= abs_other;
        }
    }
    remainder.is_negative = this->is_negative;
    if (remainder.data[0] == 0 && remainder.data[1] == 0 && remainder.data[2] == 0 && remainder.data[3] == 0) {
        remainder.is_negative = false;
    }
    return remainder;
}

Mega256& Mega256::operator+=(const Mega256& other) { *this = *this + other; return *this; }
Mega256& Mega256::operator-=(const Mega256& other) { *this = *this - other; return *this; }
Mega256& Mega256::operator*=(const Mega256& other) { *this = *this * other; return *this; }
Mega256& Mega256::operator/=(const Mega256& other) { *this = *this / other; return *this; }
Mega256& Mega256::operator%=(const Mega256& other) { *this = *this % other; return *this; }

Mega256& Mega256::operator++() { *this += Mega256(1); return *this; }
Mega256 Mega256::operator++(int) { Mega256 temp = *this; ++(*this); return temp; }
Mega256& Mega256::operator--() { *this -= Mega256(1); return *this; }
Mega256 Mega256::operator--(int) { Mega256 temp = *this; --(*this); return temp; }

bool Mega256::operator==(const Mega256& other) const {
    return this->is_negative == other.is_negative && this->data == other.data;
}

bool Mega256::operator!=(const Mega256& other) const {
    return !(*this == other);
}

bool Mega256::operator<(const Mega256& other) const {
    if (this->is_negative != other.is_negative) {
        return this->is_negative;
    }
    if (this->is_negative) {
        return other.lt_absolute(*this);
    }
    return this->lt_absolute(other);
}

bool Mega256::operator>(const Mega256& other) const { return other < *this; }
bool Mega256::operator<=(const Mega256& other) const { return !(*this > other); }
bool Mega256::operator>=(const Mega256& other) const { return !(*this < other); }

std::string Mega256::toString() const {
    char buf[256];
    snprintf(buf, sizeof(buf), "%s0x%016llX%016llX%016llX%016llX",
             is_negative ? "-" : "",
             static_cast<unsigned long long>(data[3]),
             static_cast<unsigned long long>(data[2]),
             static_cast<unsigned long long>(data[1]),
             static_cast<unsigned long long>(data[0]));
    return std::string(buf);
}

void Mega256::print() const {
    std::cout << this->toString() << std::endl;
}

std::string Mega256::toDecString() const {
    if (data[0] == 0 && data[1] == 0 && data[2] == 0 && data[3] == 0)
        return "0";
    Mega256 temp = *this;
    temp.is_negative = false;
    std::string result = "";
    while (!(temp.data[0] == 0 && temp.data[1] == 0 && temp.data[2] == 0 && temp.data[3] == 0)) {
        uint64_t remainder = 0;
        for (int i = 3; i >= 0; --i) {
            __uint128_t current = (static_cast<__uint128_t>(remainder) << 64) + temp.data[i];
            temp.data[i] = static_cast<uint64_t>(current / 10);
            remainder = static_cast<uint64_t>(current % 10);
        }
        result += std::to_string(remainder);
    }
    if (is_negative) {
        result += "-";
    }
    std::reverse(result.begin(), result.end());
    return result;
}