#pragma once
#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<unordered_set>
#include<unordered_map>
#include <string>
#include "DeterministicFiniteAutomaton.h"

std::string FindCurrentStateWithLamdaClosures(std::unordered_map<std::string, std::unordered_set<std::string>> myMap, std::unordered_set<std::string> setToFind);

class NedeterministicFiniteAutomatonLamda
{
public:

	struct UnorderedSetHash {
		std::size_t operator()(const std::unordered_set<std::string>& s) const {
			std::size_t hash = 0;
			for (const auto& str : s) {
				// Combine the hash of each element in the set
				hash ^= std::hash<std::string>{}(str)+0x9e3779b9 + (hash << 6) + (hash >> 2);
			}
			return hash;
		}
	};
private:
	std::set<std::string> m_Q;
	std::set<char> m_Sigma;
	std::map<std::pair<std::string, char>, std::vector<std::string>> m_Delta;
	std::string m_Initial;
	std::set<std::string> m_Finals;


	std::unordered_set<std::string> LamdaClosure(std::unordered_set<std::string> q);
	std::unordered_set<std::string> LamdaClosureForQ(std::string q, std::unordered_set<std::string>& closures);
	std::unordered_set<std::string> QWithCharacter(std::string q, char character);
	bool IsFinal(std::unordered_set<std::string> closures);

	bool isInResultsFromLamda(std::unordered_set<std::string> setToFind, std::unordered_map<std::unordered_set<std::string>, std::unordered_set<std::string>, UnorderedSetHash> resultFromLamdaClosure);

public:
	NedeterministicFiniteAutomatonLamda();
	NedeterministicFiniteAutomatonLamda(const NedeterministicFiniteAutomatonLamda& afn);
	NedeterministicFiniteAutomatonLamda& operator=(const NedeterministicFiniteAutomatonLamda& afn);

	//Setters
	void SetInitial(const std::string& initial);
	void SetFinals(const std::set<std::string>& finals);

	//Getters
	std::string GetInitial() const;
	std::set<std::string> GetFinals() const;

	NedeterministicFiniteAutomatonLamda Merge(NedeterministicFiniteAutomatonLamda nfa);

	void createNFA_Initial_Final_Character(std::string initial, std::set<std::string> finals, char character);
	NedeterministicFiniteAutomatonLamda connectAutomatonLamda(NedeterministicFiniteAutomatonLamda nfa, uint16_t contor);
	void modifyToLamdaTranzitions(int contor);


	void NedeterministicToDeterministic(DeterministicFiniteAutomaton& dfa);
	void PrintAutomaton();
};

