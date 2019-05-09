#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

double expression();
double term();
double power();
double factor();
double number();

int main()
{
	double n;

	setlocale(0, "");
	cout << "\n\v\tАвтор Балашов В.В.\n" << endl;
	cout << "Введите выражение тут --> ";

	n = expression();

	cout << setprecision(12) << "Результат =  " << n << endl;

	cin.get();
	cin.get();
	return 0;
}

double expression()
{
	double result;
	char operation;

	result = term();

	while (true)
	{
		operation = cin.get();

		switch (operation)
		{
		case '+':
			result += term();
			break;
		case '-':
			result -= term();
			break;
		default:
			cin.putback(operation);
			return result;
		}
	}
}

double term()
{
	double result;
	char operation;
	double temp;

	result = power();

	while (true)
	{
		operation = cin.get();

		switch (operation)
		{
		case '*':
			result *= power();
			break;
		case '/':
			temp = power();

			if (temp == 0.0)
			{
				cout << "Вы пытаетесь поделить на нуль, а это запрещено!" << endl;
				exit(-1);
			}

			result /= temp;
			break;
		default:
			cin.putback(operation);
			return result;
		}
	}
}

double power()
{
	double result;
	char operation;
	vector < double > args;

	args.push_back(factor());

	while (true)
	{
		operation = cin.get();

		while (operation == ' ')
			operation = cin.get();

		if (operation == '^')
			args.push_back(factor());
		else
		{
			cin.putback(operation);
			break;
		}
	}

	for (int i = args.size() - 1; i > 0; i--)
		args[i - 1] = pow(args[i - 1], args[i]);

	return args[0];
}

double factor()
{
	double result;
	char bracket;
	int sign = 1;

	bracket = cin.get();

	while (bracket == ' ')
		bracket = cin.get();

	switch (bracket)
	{
	case '-':
		sign = -1;
	case '+':
		bracket = cin.get();
		break;
	}

	while (bracket == ' ')
		bracket = cin.get();

	if (bracket == '(')
	{
		result = expression();
		bracket = cin.get();

		if (bracket != ')')
		{
			cout << "Вы не правильно расставили скобки!" << endl;
			exit(-1);
		}
	}
	else
	{
		cin.putback(bracket);
		result = number();
	}

	return sign * result;
}

double number()
{
	double result = 0.0;
	char digit;
	double k = 10.0;
	int sign = 1;

	digit = cin.get();

	switch (digit)
	{
	case '-':
		sign = -1;
		break;
	default:
		if (digit != '+')
			cin.putback(digit);

		break;
	}

	while (true)
	{
		digit = cin.get();

		while (digit == ' ')
			digit = cin.get();

		if (digit >= '0' && digit <= '9')
			result = result * 10.0 + (digit - '0');
		else
		{
			cin.putback(digit);
			break;
		}
	}

	digit = cin.get();

	if (digit == '.')
	{
		while (true)
		{
			digit = cin.get();

			while (digit == ' ')
				digit = cin.get();

			if (digit >= '0' && digit <= '9')
			{
				result += (digit - '0') / k;
				k *= 10.0;
			}
			else
			{
				cin.putback(digit);
				break;
			}
		}
	}
	else
		cin.putback(digit);

	return sign * result;
}
