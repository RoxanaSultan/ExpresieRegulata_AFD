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
	std::map<std::pair<std::string, char>, std::vector<std::string>> m_Delta;
	std::string m_Initial;
	std::string m_Final;
public:
	DeterministicFiniteAutomaton();
	DeterministicFiniteAutomaton(const DeterministicFiniteAutomaton& other);
	bool VerifyAutomaton();
	void PrintAutomaton();
	bool CheckWord(std::string word);

	/*void AddInQ(std::string q);
	void AddInSigma(char character);*/

	void SetQ(std::string q);
	void SetSigma(std::set<char> sigma);
	void SetFinal(std::string state);
	void SetDelta(int index, char character);
	void SetInitial(std::string state);

	std::set<std::string> GetQ();
private:
	bool IsDeterministic();
};

