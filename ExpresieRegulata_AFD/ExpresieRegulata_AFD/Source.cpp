#include <iostream>
#include"RegularExpression.h"
#include "NedeterministicFiniteAutomatonLamda.h"

int main()
{
	RegularExpression myExpression;
	std::cin >> myExpression;
	NedeterministicFiniteAutomatonLamda NFA = myExpression.CreateNFA();
	NFA.PrintAutomaton();
}