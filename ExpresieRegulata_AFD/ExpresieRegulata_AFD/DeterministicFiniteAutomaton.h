#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <set>

class DeterministicFiniteAutomaton
{
private:
	std::set<std::string> m_Q;
	std::set<char> m_Sigma;
	std::map<std::pair<std::string, char>, std::vector<std::string>> m_Delta;
	std::string m_Initial;
	std::vector<std::string> m_Finals;
public:
	DeterministicFiniteAutomaton();
	DeterministicFiniteAutomaton(const DeterministicFiniteAutomaton& other);
	bool VerifyAutomaton();
	void PrintAutomaton();
	bool CheckWord(std::string word);

	void AddInQ(std::string q);
	void AddInSigma(char character);

	void SetSigma(std::set<char> sigma);

	std::set<std::string> GetQ();
private:
	bool IsDeterministic();
};

