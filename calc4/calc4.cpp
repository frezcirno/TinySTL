#include <iostream>
#include <stack>
#include <string>
#include <cctype>
using namespace std;
/*
支持的运算符列表
单目: + -
双目: + - * / % ^ 
*/

const int prior[10] = {0,0,};

bool isunary(char op) {
	if(op=='+')
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

char comp(char op1, char op2) {
	static const char map[][10] = {
	/*栈顶\栈外	+ - + - * / ^ % */
				"",
		"",
		""
	}
}

int calc(char op, int op1) {
	switch (op) {
	case '+':
		return op1;
	case '-':
		return -op1;
	case '!':
		return !op1;
	default:
		return 0;
	}
}

int calc(char op, int op1, int op2) {
	switch (op) {
	case '+':
		return op1 + op2;
	case '-':
		return op1 - op2;
	case '*':
		return op1 * op2;
	case '/':
		return op1 / op2;
	case '&':
		return op1 & op2;
	case '|':
		return op1 | op2;
	case '^':
		return pow(op1, op2);
	case '%':
		return op1 % op2;

	default:
		return 0;
	}
}

char opname(char op, char topop) {
	if (topop == '(')
		if (op == '+') return 'p';
		else if (op == '-') return 'n';
		else return op;
	} else if (topop=='+' || topop=='-')
}

int evaluate(const char* s) {
	stack<int>opnd;
	stack<char>optr;
	optr.push('=');
	while (!optr.empty())
	{
		if (isdigit(*s)) s += readnumber(s, opnd);
		else
			switch (comp(optr.top(),*s)) {
			case '<':
				optr.push(patch(*s++));
				break;
			case '=':
				optr.pop();
				s++;
				break;
			case '>':
				char op = optr.top(); optr.pop();
				if (isunary(op)) {
					int op2 = opnd.top(); opnd.pop();
					int op1 = opnd.top(); opnd.pop();
					optr.push(calc(op, op1, op2));
				} else {
					int op1 = opnd.top(); opnd.pop();
					optr.push(calc(op, op1));
				}
				break;
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