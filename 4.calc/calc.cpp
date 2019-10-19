#include <iostream>
#include <string>
#include <cctype>
#include <cmath>
#include "../tinySTL/stack.hpp"
using namespace std;
/*
支持的运算符列表
单目: + -
双目: + - * / % ^
*/

enum optr_t
{
    EQU = 0,
    LP,
    RP,
    POS,
    NEG,
    ADD,
    SUB,
    MUL,
    DIV,
    POW,
    MOD,
    NOT,
    AND,
    OR
};


char comp(optr_t op1, optr_t op2)
{
    static const char map[][20] = {
        /*top\new "=()pn+-*d^%!&|"*/
        /* = */   "=<x<<<<<<<<<<<",
        /* ( */   "x<=<<<<<<<<<<<",
        /* ) */   ">?>xx>>>>>>x>>",
        /* p */   "><><<>>>>>><>>",
        /* n */   "><><<>>>>>><>>",
        /* + */   "><><<>><<<<<>>",
        /* - */   "><><<>><<<<<>>",
        /* * */   "><><<>>>><><>>",
        /* d */   "><><<>>>><><>>",
        /* ^ */   "><><<>>>>>><>>",
        /* % */   "><><<>>>><><>>",
        /* ~ */   "><><<>>>>>>>>>",
        /* & */   "><><<<<<<<<<>>",
        /* | */   "><><<<<<<<<<<>"
    };
    return map[op1][op2];
}

void errormsg(const char *msg){
    cout << msg << endl;
    exit(0);
}

bool isunary(optr_t op) { return (op == POS || op == NEG || op == NOT); }

int readnumber(const char* s, tinySTL::stack<int>&opnd)
{
    int x = 0, len = 0;
    while (isdigit(*s)) {
        x = 10 * x + *s - '0';
        s++;
        len++;
    }
    opnd.push(x);
    return len;
}

int calc(optr_t op, int op1)
{
    switch (op) {
    case POS:        return op1;
    case NEG:        return -op1;
    case NOT:        return ~op1;
    default:
        cout << "恭喜你发现了bug: " << op << op1 <<endl;
        return 0;
    }
}

int calc(optr_t op, int op1, int op2)
{
    switch (op) {
    case ADD:        return op1 + op2;        break;
    case SUB:        return op1 - op2;        break;
    case MUL:        return op1 * op2;        break;
    case DIV:        if(op2==0) errormsg("错误：除数为0");
                     return op1 / op2;        break;
    case AND:        return op1 & op2;        break;
    case OR:         return op1 | op2;        break;
    case POW:        return pow(op1, op2);    break;
    case MOD:        return op1 % op2;        break;

    default:
        cout << "恭喜你发现了bug: " << op << op1 << op2 <<endl;
        return -1;
        break;
    }
}

optr_t dispatch(char op, int lastObj)
{
    switch (op) {
    case '+': return ((lastObj==-1 || lastObj==RP) ? ADD : POS);        break;
    case '-': return ((lastObj==-1 || lastObj==RP) ? SUB : NEG);        break;
    case '*': return MUL;        break;
    case '/': return DIV;        break;
    case '^': return POW;        break;
    case '%': return MOD;        break;
    case '=': return EQU;        break;
    case '&': return AND;        break;
    case '|': return OR;         break;
    case '~': return NOT;        break;
    case '(':
        if(lastObj==RP) errormsg("错误：两对括号间缺少运算符");
        return LP;        break;
    case ')':
        if(lastObj==LP) errormsg("错误：括号内没有内容");
        return RP;        break;
    case '\0':
        //errormsg("错误：结尾缺少等号");
        return EQU;        break;
    default:
        errormsg("错误：未知的运算符");
    }
}

int evaluate(const char* s)
{
    tinySTL::stack<int> opnd; tinySTL::stack<optr_t> optr;
    int lastObj = EQU;
    optr.push(EQU);

    while (!optr.empty()) {
        while(isblank(*s)) s++;
        if (isdigit(*s)) {  //数字
            s += readnumber(s, opnd); lastObj = -1;
        } else {            //操作符
            optr_t newOp = dispatch(*s, lastObj);
            switch ( comp(optr.top(), newOp) ) {
            case '<': optr.push( newOp ); s++; lastObj = newOp; break;
            case '=': optr.pop(); s++; lastObj = newOp; break; //退栈
            case '>':
                {
                    optr_t aoptr = optr.top(); optr.pop();
                    if (isunary(aoptr)) {
                        int opnd1 = opnd.top(); opnd.pop();
                        opnd.push( calc(aoptr, opnd1) );
                    } else {
                        int opnd2 = opnd.top(); opnd.pop();
                        int opnd1 = opnd.top(); opnd.pop();
                        opnd.push( calc(aoptr, opnd1, opnd2) );
                    }
                }
                break;
            default:
                cout << "恭喜你发现了bug " << endl;
                return -1;
                break;
            }
        }
    }
    return opnd.top();
}


int main()
{
    string s;
    while(true){
        cout<<"输入表达式："<<endl;
        getline(cin, s);
        cout << evaluate(s.c_str()) << endl;
        cout << "是否继续（y，n）？";
        getline(cin, s);
        if(s[0] != 'y')break;
    }
    return 0;
}
