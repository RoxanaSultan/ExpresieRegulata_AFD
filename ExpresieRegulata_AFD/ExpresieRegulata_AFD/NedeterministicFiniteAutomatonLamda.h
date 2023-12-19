#pragma once
#include<iostream>
#include<vector>
#include<map>
#include<set>
#include <string>

class NedeterministicFiniteAutomatonLamda
{
private:
	std::set<std::string> m_Q;
	std::set<char> m_Sigma;
	std::map<std::pair<std::string, char>, std::vector<std::string>> m_Delta;
	std::string m_Initial;
	std::string m_Final;
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

	void PrintAutomaton();
};

