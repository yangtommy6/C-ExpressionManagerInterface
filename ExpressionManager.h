#ifndef EXPRESSION_MANAGER_H
#define EXPRESSION_MANAGER_H
#include <vector>
#include <stack>
#include <sstream>
#include <exception>
#include "ExpressionManagerInterface.h"

class ExpressionManager : public ExpressionManagerInterface
{
public:
	ExpressionManager(string& expression) : expression_(expression) {}
	virtual ~ExpressionManager() {}

	/** return the integer value of the infix expression */
	virtual int value(void);

	/** Return the infix items from the expression
		Throw an error if the expression (in this order)
		1) is missing ("Invalid Expression").
		2) is not balanced ("Unbalanced").
		3) has two adjacent operators ("Missing Operand").
		4) has an illegal operator ("Illegal Operator").
		5) has missing operator ("Missing Operator"). */
	virtual std::string infix(void);

	/** return a postfix representation of the infix expression */
	virtual std::string postfix(void);

	/** return a prefix representation of the infix expression */
	virtual std::string prefix(void);

private:
	string expression_;
	std::stack<string> operatorStack;
	string postFix_;
	string preFix_;
	const string OPEN = "([{";
	const string CLOSE = ")]}";
	const string ALL_OPERATORS = "(){}[]-+*/%";
	const int PRECEDENCE[11] = { -1, -1, -1, -1, -1, -1, 1, 1, 2, 2, 2 };
	//determine the precedence of the current operator
	int precedence(string& operator_token) const { return PRECEDENCE[ALL_OPERATORS.find(operator_token)]; }
	//reverse a string expression
	string reverseExpression(string expression);
};
#endif 

