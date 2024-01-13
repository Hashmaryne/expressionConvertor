#include <iostream>
#include<string>
#include"adtstack.h"
using namespace std;

//Class for Expression - Base Class
class CExpression
{
protected:
	string mInfixExp;
	string mOutput;

public:
	
	//setters
	void setInfix(string infix) { mInfixExp = infix; };
	void setOutput(string output) {mOutput = output; };

	//getters
	string getInfix() { return mInfixExp; };
	string getOutput() { return mOutput; };

	//pure virtual function - overriden in the derived classes
	virtual string convertExp(stackADT<char> stack, string infix) = 0;

	//returns the result of an arithmetic expression
	int evaluateExp(stackADT<int> stack, string exp, string type);

	//returns true if character passed is an alphanumeric character
	bool isAlphanumeric(char character);

	//returns true if character passed is an operator character
	bool isOperator(char character);

	//returns the level of precedence of the operator passed
	int checkPrecedence(char character);

	//returns true if all characters in the string are digits 
	bool isNumericExp(string infix);

	//returns true if character passed is a digit character
	bool isDigit(char character);

	/*returns the result after applying the passed operator to the
	 two numbers pssed*/
	int applyOperator(int num1, int num2, char chOperator);
};

//returns true if character passed is an alphanumeric character
bool CExpression::isAlphanumeric(char character)
{
	//checks if character either a letter (simple or capital) or a digit
	if ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z'))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//returns true if character passed is an operator character
bool CExpression::isOperator(char character)
{
	//checks if character passed is +,-,*,/ or ^
	if (character == '+' || character == '-' || character == '*' || character == '/' || character == '^')
	{
		return true;
	}
	else
	{
		return false;
	}
}

//returns the level of precedence of the operator passed
int CExpression::checkPrecedence(char character)
{
	//higher the precdence, higher the number returned
	if (character == '^')
		return 3;
	else if (character == '*' || character == '/')
		return 2;
	else if (character == '+' || character == '-')
		return 1;
	else
		return -1;
}

//returns true if character passed is a digit character
bool CExpression::isDigit(char character)
{
	//chceks if character is between 0 and 9
	if (character >= '0' && character <= '9')
	{
		return true;
	}
	else
	{
		return false;
	}
}

//returns true if all characters in the string are digits 
bool CExpression::isNumericExp(string infix)
{
	bool flag = true;
	//if even one character in the string is a letter false is returned
	for (int i = 0; i < infix.length(); i++)
	{
		if (isAlphanumeric(infix[i])) {
			flag = false;
		}
	}
	return flag;
}

/*returns the result after applying the passed operator to the
two numbers pssed*/
int CExpression::applyOperator(int num1, int num2, char chOperator)
{
	//applies operator depending on the character passed
	switch (chOperator)
	{
	case '^':
		return num1 ^ num2;
		break;
	case '*':
		return num1 * num2;
		break;
	case '/':
		return num1 / num2;
		break;
	case '+':
		return num1 + num2;
		break;
	case '-':
		return num1 - num2;
		break;
	}
}

//returns the result of an arithmetic expression
int  CExpression::evaluateExp(stackADT<int> stack, string exp, string type)
{
	int result = 0;
	int operand1, operand2;

	/*Scanning the expression from left to right
	 by looping through the numbers of the
	 expression*/
	for (int i = 0; i < exp.length(); i++)
	{
		/*checks if scanned character is a digit,
		if it is, character is converted into an int
		and is pushed to the stack*/
		if (isDigit(exp[i]))
		{
			stack.push(exp[i] - '0');
		}

		/*checks if scanned character is an operator,
		if it is, two operands are popped from the stack
		and the operator is applied. The result is then pushed
		to the stack*/
		else if (isOperator(exp[i]))
		{
			operand1 = stack.peek();
			stack.pop();
			operand2 = stack.peek();
			stack.pop();

			//checks if the expression is postfix or prefix
			if(type == "post")
			{
				/*if it is postfix, operand2 comes to the left of the operator and operand1 comes to the right of operator*/
				stack.push(applyOperator(operand2, operand1, exp[i]));
			}
			else if (type == "pre")
			{
				/*if it is prefix, operand1 comes to the left of the operator and operand2 comes to the right of operator*/
				stack.push(applyOperator(operand1, operand2, exp[i]));
				
			}
		}
	}
	result = stack.peek();
	return result;
}

//Postfix expression class - Derived from CExpression
class CPostfixExp : public CExpression 
{

public:

	//setters
	void setInfix(string infix) {mInfixExp = infix;};
	void setOutput(string output){mOutput = output;};

	//getters
	string getInfix() { return mInfixExp; };
	string getOutput() { return mOutput; };

	//overriden function
	string convertExp(stackADT<char> stack, string infix);
};

//overriden function
string CPostfixExp::convertExp(stackADT<char> stack, string infix) 
{
	string postfix;
	/*Scanning infix expression from left to right
	 by looping through the charcters of the
	 infix string*/
	for (int i = 0; i < infix.length(); i++)
	{
		/*checks if scanned character is alphanumeric or a digit,
		if it is, character is pushed to the stack*/
		if ((isAlphanumeric(infix[i])) || isDigit(infix[i]) )
		{
			postfix += infix[i];
		}

		/*checks if scanned character is a (,
		if it is, character is pushed to the stack*/
		else if (infix[i] == '(')
		{
			stack.push(infix[i]);
		}

		/*checks if scanned character is a (,
		if it is, the top of the stack is added to the prefix string
		 a ( is encounted. Both paranthesis are then popped*/
		else if (infix[i] == ')')
		{
			while ((stack.peek() != '(') && (!stack.isEmpty()))
			{
				postfix += stack.peek();
				stack.pop();
			}
			
			if (stack.peek() == '(')
			{
				stack.pop();
			}
		}

		/*checks if scanned character is an operator */
		else if (isOperator(infix[i]))
		{
			/*If the precedence of the scanned operator is greater than the
			precedence of the operator in the stack or the stack is empty,it
			is pushed to the stack. */
			if ((checkPrecedence(infix[i]) >= checkPrecedence(stack.peek())) || (stack.isEmpty()))
			{
				stack.push(infix[i]);
			}
			/*Else, all the operators from the stack which are greater than or equal to in precedence than that of the scanned operator is
			  popped. */
			else
			{
				while ((!stack.isEmpty()) && (checkPrecedence(infix[i]) <= checkPrecedence(stack.peek())))
				{
					postfix += stack.peek();
					stack.pop();
				}
				//Scanned character is pushed into the stack
				stack.push(infix[i]);
			}
		}
	}
	/*All the character in the stack is added to the prefix string and is then popped from the stack*/
	while (!stack.isEmpty())
	{
		postfix += stack.peek();
		stack.pop();
	}
	return postfix;
}

//Prefiix expression class - Derived from CExpression
class CPrefixExp : public CExpression
{

public:
	//setters
	void setInfix(string infix) { mInfixExp = infix; };
	void setOutput(string output){mOutput = output;};

	//getters
	string getInfix() { return mInfixExp; };
	string getOutput() { return mOutput; };

	/*overriden function - converts an infix expression 
	into a prefix expression*/
	string convertExp(stackADT<char> stack, string infix);

	//takes in a string and reverses it 
	string reverseStr(string infix);
};

//reverse function
string CPrefixExp::reverseStr(string infix)
{

	int n = infix.length();;
	stackADT<char> reverseStack(n);

	//Adds each character of the infix string into a character stack
	for (int i = 0; i < n; i++)
	{
		reverseStack.push(infix[i]);
	}

	//Pops all the characters of the character stack back into the infix string
	for (int j = 0; j < n; j++)
	{
		infix[j] = reverseStack.pop();
	}
	return infix;
} 

string CPrefixExp::convertExp(stackADT<char> stack, string infix)
{
	string prefix;
	//reverse function is called to reverse infix expression
	infix=reverseStr(infix);
	//reverses '(' to ')' and vice versa
	for (int i = 0; i < infix.length(); i++) {
		if (infix[i] == '(') {
			infix[i] = ')';
		}
		else if (infix[i] == ')') {
			infix[i] = '(';
		}
	}

	/*Scanning infix expression from left to right
	 by looping through the charcters of the
	 infix string*/
	for (int i = 0; i < infix.length(); i++)
	{
		/*checks if scanned character is alphanumeric or a digit,
		if it is, character is pushed to the stack*/
		if ((isAlphanumeric(infix[i])) || (isDigit(infix[i])))
		{
			prefix += infix[i];
		}

		/*checks if scanned character is a (,
		if it is, character is pushed to the stack*/
		else if (infix[i] == '(')
		{
			stack.push(infix[i]);
		}

		/*checks if scanned character is a (,
		if it is, the top of the stack is added to the prefix string
		 a ( is encounted. Both paranthesis are then popped*/
		else if (infix[i] == ')') 
		{
			while ((stack.peek() != '(') && (!stack.isEmpty())) 
			{
				prefix += stack.peek();
				stack.pop();
			}

			if (stack.peek() == '(') {
				stack.pop();
			}
		}

		/*checks if scanned character is an operator */
		else if (isOperator(infix[i]))
		{
			/*If the precedence of the scanned operator is greater than the
			precedence of the operator in the stack or the stack is empty,it
			is pushed to the stack. */
			if ((checkPrecedence(infix[i]) > checkPrecedence(stack.peek())) || (stack.isEmpty()))
			{
				stack.push(infix[i]);
			}
			/*Else, all the operators from the stack which are greater than or equal to in
			precedence than that of the scanned operator is popped. */
			else
			{
				while ((!stack.isEmpty()) && (checkPrecedence(infix[i]) <= checkPrecedence(stack.peek())))
				{
					prefix += stack.peek();
					stack.pop();
				}
				//Scanned character is pushed into the stack
				stack.push(infix[i]);
			}
		}
	}

	/*All the character in the stack is added to the prefix string and is then popped from the stack*/
	while (!stack.isEmpty())
	{
		prefix += stack.peek();
		stack.pop();
	}

	//expression is reversed back
	prefix=reverseStr(prefix);
	return prefix;
}

//Driver class
class CProgram 
{
private:
	int choice;
	int expSize;

public:
	int getExpSize() { return expSize; };
	//displays menu
	int displayMenu();
	//does conversion based on user input
	void doConversion();
};

int CProgram::displayMenu() 
{
	cout << "Select an option" << endl;
	cout << "1. Inifx to Postfix\n2. Infix to Prefix\n3. Exit" << endl;
	cout << "Your choice: ";
	cin >> choice;
	if (choice == 1 || choice == 2 )
	{
		doConversion(); // if user selects Infix to Postfix or Infix to Prefix
		return 1;
		
	}
	else if (choice == 3) // if user selects exit
	{
		system("CLS");
		cout << "Application closed" << endl;
		return 0; // returns 0 if exits
	}
	else //When user enters an invalid input
	{
		system("CLS");
		cout << "Invalid input. Please try again\n" << endl;
		displayMenu();//calls same function again
		return 1;
	}
	
}

void CProgram::doConversion()
{
	string infix, output;
	cout << "Enter a Infix Expression : ";
	//user input
	cin >> infix;
	/*The sizes of the stacks will be the length of the 
	 infix expression input by the user*/
	expSize=infix.length();
	stackADT<char> charStack(expSize); // stack for expression conversion
	stackADT<int> intStack(expSize); // stack for expression evaluation
	int result;

	if (choice == 1) // Choice to convert infix to postfix
	{
		CPostfixExp postfixExp; // object of CPostfixExp
		postfixExp.setInfix(infix); // setting the infix to the user input
		/*converts the postfix expressin by taking in the char stack and the infix expression*/
		output = postfixExp.convertExp(charStack, postfixExp.getInfix());
		postfixExp.setOutput(output);// setting the converted expression as the output
		cout << "Postfix Expression : " << postfixExp.getOutput() << endl;

		/*if the expression is numeric, the expression will be evaluated
		to get the result*/
		if(postfixExp.isNumericExp(postfixExp.getInfix()))
		{
			result = postfixExp.evaluateExp(intStack, output,"post");
			cout << "Result after evaluation : " << result << endl;
		}
	}
	else if(choice == 2) // Choice to convert infix to prefix
	{
		CPrefixExp prefixExp; // object of CPrefixExp
		prefixExp.setInfix(infix); // setting the infix to the user input
		/*converts the prefix expressin by taking in the char stack and the infix expression*/
		output = prefixExp.convertExp(charStack, prefixExp.getInfix());
		prefixExp.setOutput(output);
		cout << "Prefix Expression : " << prefixExp.getOutput() << endl;

		/*if the expression is numeric, the expression will be evaluated
		to get the result*/
		if (prefixExp.isNumericExp(prefixExp.getInfix()))
		{
			//reverse output to evaluate the prefix expression
			output=(prefixExp.reverseStr(prefixExp.getOutput()));
			result = prefixExp.evaluateExp(intStack, output,"pre");

			cout << "Result after evaluation : " << result << endl;
		}
	}
	system("pause");
	system("CLS");
	//display menu again after conversion and evaluation is done
	displayMenu();

}

int main()
{
	//Object to create program
	CProgram program;

	//function to display code is called. If 0 is returned application will exit
	if (program.displayMenu() == 0) 
	{
		system("pause");
		exit(0);
	};
	
	return 0;
}