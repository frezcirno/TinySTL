#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <cmath>
//#define DEBUG
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
    char ch[20]="=()pn+-*d^%~&|";
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

#ifdef DEBUG
    cout<<"Compare:"<<ch[op1]<<" "<<map[op1][op2]<<" "<<ch[op2]<<endl;
#endif
    return map[op1][op2];
}

void errormsg(const char*msg){
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
        return ~op1;
        break;

    default:
        cout << "恭喜你发现了bug: " << op << op1 <<endl;
        return 0;
        break;
    }
}

int calc(Optr op, int op1, int op2)
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

Optr dispatch(char op, int lastObj)
{
#ifdef DEBUG
    cout<<op<<" "<<lastObj<<endl;
#endif // DEBUG
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
        return 0;
        break;
    }
}

int evaluate(const char* s)
{
    stack<int> opnd; stack<Optr> optr;
    Optr lastObj = EQU;
    optr.push(EQU);

    while (!optr.empty()) {
        while(isblank(*s)) s++;
        if (isdigit(*s)) {  //数字
            s += readnumber(s, opnd); lastObj = -1;
#ifdef DEBUG
            cout<<"Read number"<<opnd.top()<<endl;
#endif
        } else {            //操作符
            Optr newOp = dispatch(*s, lastObj);
#ifdef DEBUG
            cout<<"=()pn+-*d^%!&|"[newOp]<<endl;
#endif
            switch ( comp(optr.top(), newOp) ) {
            case '<': optr.push( newOp ); s++; lastObj = newOp; break;
            case '=': optr.pop(); s++; lastObj = newOp; break; //退栈
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
