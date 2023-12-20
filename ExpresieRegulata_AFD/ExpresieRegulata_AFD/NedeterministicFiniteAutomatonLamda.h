#pragma once
#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<unordered_set>
#include<unordered_map>
#include <string>
#include "DeterministicFiniteAutomaton.h"

class NedeterministicFiniteAutomatonLamda
{
private:
	std::set<std::string> m_Q;
	std::set<char> m_Sigma;
	std::map<std::pair<std::string, char>, std::vector<std::string>> m_Delta;
	std::string m_Initial;
	std::string m_Final;


	std::unordered_set<std::string> LamdaClosure(std::unordered_set<std::string> q);
	std::unordered_set<std::string> LamdaClosureForQ(std::string q);
	std::unordered_set<std::string> QWithCharacter(std::unordered_set<std::string>, char character);

public:
	NedeterministicFiniteAutomatonLamda();
	NedeterministicFiniteAutomatonLamda(const NedeterministicFiniteAutomatonLamda& afn);
	NedeterministicFiniteAutomatonLamda& operator=(const NedeterministicFiniteAutomatonLamda& afn);

	//Setteri
	void SetInitial(const std::string& initial);
	void SetFinal(const std::string & final);

	//Getteri
	std::string GetInitial() const;
	std::string GetFinal() const;

	NedeterministicFiniteAutomatonLamda Merge(NedeterministicFiniteAutomatonLamda nfa);

	void createNFA_Initial_Final_Character(std::string initial, std::string finals, char character);
	NedeterministicFiniteAutomatonLamda connectAutomatonLamda(NedeterministicFiniteAutomatonLamda nfa, uint16_t contor);
	void modifyTpLamdaTranzitions(int contor);


	void NedeterministicToDeterministic(DeterministicFiniteAutomaton dfa);
	void PrintAutomaton();
};

