/**
 * 计算器测试
 * 
 * 
*/

#define main not_main
#include "calc.cpp"
#undef main

#define TEST(a, b)                               \
    cout << ++cases << ". " << a << evaluate(a); \
    if (evaluate(a) == b)                        \
        pass++;                                  \
    cout << endl;

void testcase()
{
    int cases = 0;
    int pass = 0;
    TEST("0=", 0);
    TEST("1=", 1);
    TEST("+1=", 1);
    TEST("-1=", -1);
    TEST("+-1=", -1);
    TEST("-+1=", -1);
    TEST("--1=", 1);

    TEST("1+1=", 2);
    TEST("1-1=", 0);
    TEST("+1+1=", 2);
    TEST("-1+1=", 0);
    TEST("-1-1=", -2);
    TEST("1-2=", -1);

    TEST("(1)=", 1);
    TEST("-(1)=", -1);
    TEST("-(-1)=", 1);

    TEST("1*1=", 1);
    TEST("0*1=", 0);
    TEST("1*0=", 0);
    TEST("0*0=", 0);
    TEST("2/1=", 2);
    TEST("4/2=", 2);
    TEST("1/1=", 1);

    TEST("(1^10000)=", 1);
    TEST("2^10=", 1024);
    TEST("(2)*(2)=", 4);
    TEST("(2/1)=", 2);
    TEST("2^10=", 1024);
    TEST("2%10=", 2);
    TEST("11%10=", 1);
    TEST("10000007%10=", 7);

    TEST("1&2&4&8&16&32&64=", 0);
    TEST("1|2|4|8|16|32|64=", 127);
    TEST("~-1=", 0);
    TEST("~0=", -1);
    TEST("~1=", -2);
    TEST("~2=", -3);
    TEST("~(~2)=", 2);

    TEST("(1+(-1+(-3+4))*(1))=", 1);
    TEST("-2*(3+5)+2^3/4=", -14);
    TEST("2^4/8-(+2+8)%3=", 1);

    cout << endl
         << pass << "/" << cases << "=" << (double)pass / cases * 100 << "%" << endl;
}

int main()
{
    testcase();
    return 0;
}
