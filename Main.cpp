#include <iostream>
#include <fstream>
#include <sstream>
#include "ExpressionManager.h"

using namespace std;

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		cerr << "Please provide name of input and output files";
		return 1;
	}
	cout << "Input file: " << argv[1] << endl;
	ifstream in(argv[1]);
	if (!in)
	{
		cerr << "Unable to open " << argv[1] << " for input";
		return 2;
	}
	cout << "Output file: " << argv[2] << endl;
	ofstream out(argv[2]);
	if (!out)
	{
		in.close();
		cerr << "Unable to open " << argv[2] << " for output";
		return 3;
	}

	string line, command;
	while (getline(in, line))
	{
		try
		{
			if (line.size() == 0) continue;
			string expression_line = line.substr(line.find(' ') + 1);
			ExpressionManager expressionManager(expression_line);
			out << "Expression: " << expression_line << endl;
			while (in >> line)
			{
				if (line == "Infix:")
				{
					out << "Infix: ";
					out << expressionManager.infix() << endl;
				}
				else if (line == "Postfix:")
				{
					out << "Postfix: ";
					out << expressionManager.postfix() << endl;
				}
				else if (line == "Prefix:")
				{
					out << "Prefix: ";
					out << expressionManager.prefix() << endl;
				}
				else if (line == "Value:")
				{
					out << "Value: ";
					out << expressionManager.value() << endl;
				}
				else
				{
					break;
				}
			}
		}
		catch (string exceptionString)
		{
			out << exceptionString << endl;
		}
		out << endl;
	}

	system("pause");
	return 0;
}