#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <cmath>
using namespace std;
/*
支持的运算符列表
单目: + -
双目: + - * / % ^ 
*/
#define EQU 0
#define LP 1 
#define RP 2 
#define POS 3
#define NEG 4
#define ADD 5
#define SUB 6
#define MUL 7
#define DIV 8
#define POW 9
#define MOD 10
#define NOT 11
#define AND 12
#define OR 13
char comp(int op1, int op2) {
    static const char map[][20] = {
/*top\new*/ /* =()pn+-*d^%!&| */
    /* = */   "=<<<<<<<<<<<<<",
    /* ( */   "x<=<<<<<<<<<<<",
    /* ) */   ">?>xx>>>>>>>>>",
    /* p */   "><x<<>>>>>>???",
    /* n */   "><x<<>>>>>>???",
    /* + */   "><><<>><<<<   ",
    /* - */   "><><<>><<<<   ",
    /* * */   "><><<>>>><>   ",
    /* d */   "><><<>>>><>   ",
    /* ^ */   "><><<>>>>>>   ",
    /* % */   "><><<>>>><>   ",
    /* ! */   "",
    /* & */   "",
    /* | */   ""
    };
    return map[op1][op2];
}


bool isunary(int op) {
    return (op == POS || op == NEG ||
            op == NOT);
}

int readnumber(const char* s, stack<int>opnd) {
    int x = 0, len = 0;
    while (isdigit(*s))
    {
        x = 10 * x + *s - '0';
        s++; len++;
    }
    opnd.push(x);
    return len;
}

int calc(int op, int op1) {
    switch (op) {
    case POS:
        return op1;
    case NEG:
        return -op1;
    case NOT:
        return !op1;
    default:
        return 0;
    }
}

int calc(int op, int op1, int op2) {
    switch (op) {
    case ADD:
        return op1 + op2;
    case SUB:
        return op1 - op2;
    case MUL:
        return op1 * op2;
    case DIV:
        return op1 / op2;
    case AND:
        return op1 & op2;
    case OR:
        return op1 | op2;
    case POW:
        return pow(op1, op2);
    case MOD:
        return op1 % op2;

    default:
        return 0;
    }
}

char patch(int op, bool lastIsOp) {
    switch (op) {
        case '+': return (lastIsOp ? POS : ADD); break;
        case '-': return (lastIsOp ? NEG : SUB); break;
        case '*': return MUL; break;
        case '/': return DIV; break;
        case '^': return POW; break;
        case '%': return MOD; break;
        case '=': return EQU; break;
        case '&': return AND; break;
        case '|': return OR; break;
        case '!': return NOT; break;
        case '(': return LP; break;
        case ')': return RP; break;
        default: cout << "Error: " << op <<endl;
    }
}
int evaluate(const char* s) {
    stack<int> opnd, optr;
    bool lastIsOp = true; 
    optr.push('=');
    while (!optr.empty())
    {
        if (isblank(*s)) { s++; continue; } //跳过空格
        if (isdigit(*s)) { s += readnumber(s, opnd); lastIsOp = false; }
        else {//操作符
            switch ( comp(optr.top(), *s) ) {
            case '<':
                optr.push(patch(*s++, lastIsOp));
                break;
            case '=': //退栈
                optr.pop();
                s++;
                break;
            case '>':
                int op = optr.top(); optr.pop();
                if (isunary(op)) {
                    int op2 = opnd.top(); opnd.pop();
                    int op1 = opnd.top(); opnd.pop();
                    optr.push(calc(op, op1, op2));
                } else {
                    int op1 = opnd.top(); opnd.pop();
                    optr.push(calc(op, op1));
                }
                break;
            default:
                cout << "Error: " << op << endl;
                break;
            }
            lastIsOp = true;
        }
    }
    return opnd.top();
}

int main() {
    string s;
    getline(cin, s);
    evaluate(s.c_str());

    return 0;
}
