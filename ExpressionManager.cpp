#include "ExpressionManager.h"
#include <iostream>

int ExpressionManager::value(void)
{
	infix();
	postFix_ = postfix();
	std::stack<int> operand_stack;
	std::istringstream iss(postFix_);
	string next_token;
	while (iss >> next_token)
	{
		//push all the digits
		if (isdigit(next_token[0]))
		{
			int value = std::stoi(next_token);
			operand_stack.push(value);
		}
		//calculate the result of the previous two digits when an operator is encountered. Push the result.
		else
		{
			int leftOperand = operand_stack.top();
			operand_stack.pop();
			int rightOperand = operand_stack.top();
			operand_stack.pop();
			int result = 0;
			switch (next_token[0])
			{
			case '+':
				result = rightOperand + leftOperand;
				break;
			case '-':
				result = rightOperand - leftOperand;
				break;
			case '*':
				result = rightOperand * leftOperand;
				break;
			case '/':
				result = rightOperand / leftOperand;
				break;
			case '%':
				result = rightOperand % leftOperand;
				break;
			}
			operand_stack.push(result);
		}
	}
	return operand_stack.top();
}

string ExpressionManager::infix(void)
{
	std::stack<string> string_stack;
	std::istringstream iss(expression_);
	string next_token;
	while (iss >> next_token)
	{
		//push open paren
		if (OPEN.find(next_token) != string::npos)
		{
			string_stack.push(next_token);
		}
		else if (CLOSE.find(next_token) != string::npos)
		{
			//check extra close paren
			if (string_stack.empty())
			{
				throw std::string("Unbalanced");
			}
			else
			{
				string top_string = string_stack.top();
				string_stack.pop();
				// check if the open paren and the close paren match
				if (OPEN.find(top_string) != CLOSE.find(next_token)) { throw std::string("Unbalanced"); }
			}
		}
	}
	//check if extra open paren
	if (!string_stack.empty())
	{
		throw std::string("Unbalanced");
	}

	const string OPERATORS_WITHOUT_PAREN = "-+*/%";
	while (!string_stack.empty())
	{
		string_stack.pop();
	}
	std::istringstream iss_two(expression_);
	while (iss_two >> next_token)
	{
		//check if there's an illegal operator
		if ((!isdigit(next_token[0])) && (ALL_OPERATORS.find(next_token) == string::npos))
		{
			throw std::string("Illegal Operator");
		}
		if (isdigit(next_token[0]))
		{
			//check if there are two adjacent digits
			if (!string_stack.empty())
			{
				throw std::string("Missing Operator");
			}
			string_stack.push(next_token);
		}
		else if (OPERATORS_WITHOUT_PAREN.find(next_token) != string::npos)
		{
			//check extra operator
			if (string_stack.empty())
			{
				throw std::string("Missing Operand");
			}
			string_stack.pop();
		}
	}
	//check if there is a digit left in the stack
	if (string_stack.empty())
	{
		throw std::string("Missing Operand");
	}

	return expression_;
}

string ExpressionManager::postfix(void)
{
	infix();
	postFix_ = "";
	while (!operatorStack.empty())
	{
		operatorStack.pop();
	}
	std::istringstream iss(expression_);
	string next_token;
	while (iss >> next_token)
	{
		//append digits to the postfix string
		if (isdigit(next_token[0]))
		{
			postFix_ += next_token;
			postFix_ += " ";
		}
		//process operators
		else if (ALL_OPERATORS.find(next_token) != string::npos)
		{
			if (operatorStack.empty() || OPEN.find(next_token) != string::npos)
			{
				operatorStack.push(next_token);
			}
			else
			{
				if (precedence(next_token) > precedence(operatorStack.top()))
				{
					operatorStack.push(next_token);
				}
				else
				{
					/*append operators to the postfix string until the stack is empty or
					the current operator precedence is higher or there is an open paren*/
					while (!operatorStack.empty() && (precedence(next_token) <= precedence(operatorStack.top()))
						&& OPEN.find(operatorStack.top()) == string::npos)
					{
						postFix_ += operatorStack.top();
						postFix_ += " ";
						operatorStack.pop();
					}
					//pop the open paren if the next token is a close paren
					if (CLOSE.find(next_token) != string::npos) { operatorStack.pop(); }
					else { operatorStack.push(next_token); }
				}
			}
		}
	}
	//append operators in the stack to the postfix string
	while (!operatorStack.empty())
	{
		string op = operatorStack.top();
		operatorStack.pop();
		postFix_ += op;
		postFix_ += " ";
	}
	return postFix_;
}

string ExpressionManager::prefix(void)
{
	infix();
	string next_token;

	/* Step one: call reverse function - use a string stack to store strings (Open paren will become close paren, and close paren will become
	open paren) to get the reversed expression */
	string reversedExpression = reverseExpression(expression_);

	/* Step two: same method used in postfix except that the operator stack will push the current operator if it has
	the same precedence as the top operator in the stack */
	postFix_ = "";
	while (!operatorStack.empty())
	{
		operatorStack.pop();
	}
	std::istringstream iss(reversedExpression);
	while (iss >> next_token)
	{
		//append digits to the postfix string
		if (isdigit(next_token[0]))
		{
			postFix_ += next_token;
			postFix_ += " ";
		}
		//process operators
		else if (ALL_OPERATORS.find(next_token) != string::npos)
		{
			if (operatorStack.empty() || OPEN.find(next_token) != string::npos)
			{
				operatorStack.push(next_token);
			}
			else
			{
				if (precedence(next_token) >= precedence(operatorStack.top()) && CLOSE.find(next_token) == string::npos)
				{
					operatorStack.push(next_token);
				}
				else
				{
					/*append operators to the postfix string until the stack is empty or
					the current operator precedence is higher or there is an open paren*/
					while (!operatorStack.empty() && (precedence(next_token) < precedence(operatorStack.top()))
						&& OPEN.find(operatorStack.top()) == string::npos)
					{
						postFix_ += operatorStack.top();
						postFix_ += " ";
						operatorStack.pop();
					}
					//pop the open paren if the next token is a close paren
					if (CLOSE.find(next_token) != string::npos) { operatorStack.pop(); }
					else { operatorStack.push(next_token); }
				}
			}
		}
	}
	//append operators in the stack to the postfix string
	while (!operatorStack.empty())
	{
		string op = operatorStack.top();
		operatorStack.pop();
		postFix_ += op;
		postFix_ += " ";
	}

	//Final step: call reverse function - reverse the postfix expression that's been converted from the reversed expression to get prefix
	preFix_ = reverseExpression(postFix_);

	return preFix_;
}


string ExpressionManager::reverseExpression(string expression)
{
	string reversedExpression = "";
	std::stack<string> string_stack;
	std::istringstream iss(expression);
	string next_token;
	while (iss >> next_token)
	{
		if (CLOSE.find(next_token) != string::npos)
		{
			string_stack.push(OPEN.substr(CLOSE.find(next_token), 1));
		}
		else if (OPEN.find(next_token) != string::npos)
		{
			string_stack.push(CLOSE.substr(OPEN.find(next_token), 1));
		}
		else
		{
			string_stack.push(next_token);
		}
	}
	//get the reversed string
	while (!string_stack.empty())
	{
		reversedExpression += string_stack.top();
		reversedExpression += " ";
		string_stack.pop();
	}
	return reversedExpression;
}