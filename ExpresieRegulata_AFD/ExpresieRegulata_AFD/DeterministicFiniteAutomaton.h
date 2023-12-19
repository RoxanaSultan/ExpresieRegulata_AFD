#pragma once

#include <iostream>
#include <vector>
#include <map>

class DeterministicFiniteAutomaton
{
private:
	std::vector<std::string> m_Q;
	std::vector<char> m_Sigma;
	std::map<std::pair<std::string, char>, std::vector<std::string>> m_Delta;
	std::string m_Initial;
	std::vector<std::string> m_Finals;
public:
	DeterministicFiniteAutomaton();
	DeterministicFiniteAutomaton(const DeterministicFiniteAutomaton& other);
	bool VerifyAutomaton();
	void PrintAutomaton();
	bool CheckWord(std::string word);
private:
	bool IsDeterministic();
};

