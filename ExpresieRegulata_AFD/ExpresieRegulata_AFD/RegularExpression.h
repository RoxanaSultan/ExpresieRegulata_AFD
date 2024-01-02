#pragma once
#include "NedeterministicFiniteAutomatonLamda.h"
#include "DeterministicFiniteAutomaton.h"
#include "PolishNotation.h"
#include <string>



class RegularExpression
{
private:
	std::string m_word;
	PolishNotation m_PolishNotation;


public:
	RegularExpression() = default;
	RegularExpression(const RegularExpression& expression);
	RegularExpression& operator=(const RegularExpression& expression);
	void ExpressionToPolishNotation();
	NedeterministicFiniteAutomatonLamda CreateNFA();
	bool IsValid();
	friend std::istream& operator>>(std::istream& in, RegularExpression& expression);
};

