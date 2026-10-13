#ifndef MEGA256_MEGA256_H
#define MEGA256_MEGA256_H

#include <cstdint>
#include <string>
#include <array>

struct Mega256 {
    std::array<uint64_t, 4> data;
    bool is_negative;

    static const Mega256 MAX_VALUE;
    static const Mega256 MIN_VALUE;

    Mega256();
    Mega256(int64_t value);
    Mega256(uint64_t value);
    Mega256(const std::string& str);

    Mega256 operator+(const Mega256& other) const;
    Mega256 operator-(const Mega256& other) const;
    Mega256 operator*(const Mega256& other) const;
    Mega256 operator/(const Mega256& other) const;
    Mega256 operator%(const Mega256& other) const;

    Mega256& operator+=(const Mega256& other);
    Mega256& operator-=(const Mega256& other);
    Mega256& operator*=(const Mega256& other);
    Mega256& operator/=(const Mega256& other);
    Mega256& operator%=(const Mega256& other);

    Mega256& operator++();
    Mega256 operator++(int);
    Mega256& operator--();
    Mega256 operator--(int);

    bool operator==(const Mega256& other) const;
    bool operator!=(const Mega256& other) const;
    bool operator<(const Mega256& other) const;
    bool operator>(const Mega256& other) const;
    bool operator<=(const Mega256& other) const;
    bool operator>=(const Mega256& other) const;

    [[nodiscard]] std::string toString() const;
    void print() const;
    [[nodiscard]] std::string toDecString() const;

private:
    void shiftLeftOne();
    Mega256 add_absolute(const Mega256& other) const;
    Mega256 sub_absolute(const Mega256& other) const;
    bool lt_absolute(const Mega256& other) const;
};

#endif