#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <string>

class DeterministicFiniteAutomaton
{
private:
	std::set<std::string> m_Q;
	std::set<char> m_Sigma;
	std::map<std::pair<std::string, char>, std::string> m_Delta;
	std::string m_Initial;
	std::set<std::string> m_Finals;
public:
	DeterministicFiniteAutomaton();
	DeterministicFiniteAutomaton(const DeterministicFiniteAutomaton& other);
	bool VerifyAutomaton();
	void PrintAutomaton();
	bool CheckWord(std::string word);

	void SetQ(std::string q);
	void SetSigma(std::set<char> sigma);
	void SetFinals(std::set<std::string> finals);
	void SetDelta(std::string Key, char character, std::string Value);
	void SetInitial(std::string state);

	std::set<std::string> GetQ();
};

