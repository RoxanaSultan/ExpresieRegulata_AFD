#pragma once
#include <queue>
#include <stack>
#include <iostream>

class PolishNotation
{
private:
	std::queue<char> m_polishNotation;

	bool IsOperator(const char& character);
	int OperatorRank(const char& character);

public:
	std::queue<char> CreatePolishNotation(std::string expression);

	//Getter
	std::queue<char> GetQueuePolishNotation();

};

