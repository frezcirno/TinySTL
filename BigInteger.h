#pragma once
#include <string>

class BigInteger
{
  public:
    static BigInteger ZERO;
    static BigInteger ONE;
    static BigInteger TEN;

  public:
    BigInteger() {}
    BigInteger(const char *val) {}

    ~BigInteger() {}

    BigInteger abs();

    BigInteger add(const BigInteger &val);
    BigInteger subtract(const BigInteger &val);
    BigInteger multiply(const BigInteger &val);
    BigInteger divide(const BigInteger &val);
    BigInteger mod(const BigInteger &val);
    BigInteger remainder(const BigInteger &val);
    BigInteger divideAndRemainder(const BigInteger &val);
    BigInteger negate();
    BigInteger pow(int exponent);

    BigInteger not_();
    BigInteger or_(const BigInteger &val);
    BigInteger and_(const BigInteger &val);
    BigInteger xor_(const BigInteger &val);
    BigInteger shiftLeft(int n);
    BigInteger shiftRight(int n);

    int compareTo(const BigInteger &val);
    bool equals(const BigInteger &val);

    std::string toString();
    int intValue();
    long longValue();

  private:
    int signum = 0;      //符号位
    int *mag = nullptr;  //数值位
    int bitCount = -1;   //
    int bitLength = -1;  //数字总位数(二进制)

};
