#pragma once
#include<iostream>
#include<vector>
#include<map>
class NedeterministicFiniteAutomatonLamda
{
private:
	std::vector<std::string> m_Q;
	std::vector<char> m_Sigma;
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
};

