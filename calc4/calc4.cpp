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

typedef int Optr;

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

char comp(Optr op1, Optr op2)
{
    char ch[20]="=()pn+-*d^%!&|";
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
        /* ! */   "><><<>>>>>>>>>",
        /* & */   "><><<<<<<<<<>>",
        /* | */   "><><<<<<<<<<<>"
    };

    cout<<"Compare:"<<ch[op1]<<" "<<map[op1][op2]<<" "<<ch[op2]<<endl;
    return map[op1][op2];
}

void errormsg(char*msg){
    cout<<msg<<endl;
    exit(0);
}

bool isunary(Optr op)
{
    return (op == POS || op == NEG ||
            op == NOT);
}

int readnumber(const char* s, stack<int>&opnd)
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

int calc(Optr op, int op1)
{
    switch (op) {
    case POS:
        return op1;
        break;
    case NEG:
        return -op1;
        break;
    case NOT:
        return !op1;
        break;

    default:
        cout << "Calc1 Error: " << op << op1 <<endl;
        return 0;
        break;
    }
}

int calc(Optr op, int op1, int op2)
{
    switch (op) {
    case ADD:
        return op1 + op2;
        break;
    case SUB:
        return op1 - op2;
        break;
    case MUL:
        return op1 * op2;
        break;
    case DIV:
        return op1 / op2;
        break;
    case AND:
        return op1 & op2;
        break;
    case OR:
        return op1 | op2;
        break;
    case POW:
        return pow(op1, op2);
        break;
    case MOD:
        return op1 % op2;
        break;

    default:
        cout << "Calc2 Error: " << op << op1 << op2 <<endl;
        return -1;
        break;
    }
}

Optr dispatch(char op, bool lastIsOp, Optr &lastOp)
{
    switch (op) {
    case '+':
        return lastOp=((lastIsOp && lastOp!=RP) ? POS : ADD);
        break;
    case '-':
        return lastOp=((lastIsOp && lastOp!=RP) ? NEG : SUB);
        break;
    case '*':
        return lastOp=MUL;
        break;
    case '/':
        return lastOp=DIV;
        break;
    case '^':
        return lastOp=POW;
        break;
    case '%':
        return lastOp=MOD;
        break;
    case '=':
        return lastOp=EQU;
        break;
    case '&':
        return lastOp=AND;
        break;
    case '|':
        return lastOp=OR;
        break;
    case '!':
        return lastOp=NOT;
        break;
    case '(':
        return lastOp=LP;
        break;
    case ')':
        if(lastIsOp && lastOp==LP) errormsg("错误：括号内没有内容");
        return lastOp=RP;
        break;
    default:
        errormsg("错误：未知的运算符");
        break;
    }
}

int evaluate(const char* s)
{
    stack<int> opnd; stack<Optr> optr;
    bool lastIsOp = true;
    Optr lastOp = EQU;
    optr.push(EQU);
    while (!optr.empty()) {
        if (isblank(*s)) {
            s++;    //跳过空格
            continue;
        }
        if (isdigit(*s)) {
            s += readnumber(s, opnd); lastIsOp = false;
        } else { //操作符
            int newOp = dispatch(*s, lastIsOp, lastOp);
            switch ( comp(optr.top(), newOp) ) {
            case '<':
                optr.push( newOp );
                s++;
                break;
            case '=': //退栈
                optr.pop();
                s++;
                break;
            case '>':
                {
                    Optr aoptr = optr.top(); optr.pop();
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
                cout << "Comp Error. " << endl;
                return -1;
                break;
            }
            lastIsOp = true;
        }
    }
    return opnd.top();
}

int main()
{
    string s;
    getline(cin, s);
    cout << evaluate(s.c_str()) << endl;
    return 0;
}
