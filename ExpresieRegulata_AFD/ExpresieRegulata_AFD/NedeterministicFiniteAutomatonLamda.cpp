#include "NedeterministicFiniteAutomatonLamda.h"

std::unordered_set<std::string> NedeterministicFiniteAutomatonLamda::LamdaClosure(std::unordered_set<std::string> q)
{
	std::unordered_set<std::string> closures;
	for (auto it : q)
	{
		auto newUnordered_set = LamdaClosureForQ(it);
		closures.insert(newUnordered_set.begin(), newUnordered_set.end());
		closures.insert(it);
	}
	
	return closures;
}

std::unordered_set<std::string> NedeterministicFiniteAutomatonLamda::LamdaClosureForQ(std::string q)
{
	std::unordered_set<std::string> closures;
	if(find(m_Delta.begin(), m_Delta.end(), std::make_pair(q, '#')) != m_Delta.end())
	{
		auto myVector = find(m_Delta.begin(), m_Delta.end(), std::make_pair(q, '#'))->second;
		closures.insert(myVector.begin(), myVector.end());
		for (auto it : myVector)
		{
			auto newSet = LamdaClosureForQ(it);
			closures.insert(newSet.begin(), newSet.end());
		}
	}
	
	return closures;
}



NedeterministicFiniteAutomatonLamda::NedeterministicFiniteAutomatonLamda()
{
	m_Sigma.insert('#');
}

NedeterministicFiniteAutomatonLamda::NedeterministicFiniteAutomatonLamda(const NedeterministicFiniteAutomatonLamda& afn) :
	m_Q{ afn.m_Q },
	m_Sigma{ afn.m_Sigma },
	m_Delta{ afn.m_Delta },
	m_Initial{ afn.m_Initial },
	m_Final{ afn.m_Final }
{}

NedeterministicFiniteAutomatonLamda& NedeterministicFiniteAutomatonLamda::operator=(const NedeterministicFiniteAutomatonLamda& afn)
{
	m_Q = afn.m_Q;
	m_Sigma = afn.m_Sigma;
	m_Delta = afn.m_Delta;
	m_Initial = afn.m_Initial;
	m_Final = afn.m_Final;
	return *this;
}

void NedeterministicFiniteAutomatonLamda::SetInitial(const std::string& initial)
{
	m_Initial = initial;
	if (std::ranges::find(m_Q, initial) == m_Q.end())
		m_Q.insert(initial);
}

void NedeterministicFiniteAutomatonLamda::SetFinal(const std::string & final)
{
	m_Final = final;
	if (std::ranges::find(m_Q, final) == m_Q.end())
		m_Q.insert(final);
}

std::string NedeterministicFiniteAutomatonLamda::GetInitial() const
{
	return m_Initial;
}

std::string NedeterministicFiniteAutomatonLamda::GetFinal() const
{
	return m_Final;
}

NedeterministicFiniteAutomatonLamda NedeterministicFiniteAutomatonLamda::Merge(NedeterministicFiniteAutomatonLamda nfa)
{
	m_Q.insert(nfa.m_Q.begin(), nfa.m_Q.end());
	m_Q.erase(std::ranges::find(m_Q, nfa.m_Initial));
	m_Q.erase(std::ranges::find(m_Q, m_Final));
	m_Q.insert(m_Final + nfa.m_Initial);
	m_Sigma.insert(nfa.m_Sigma.begin(), nfa.m_Sigma.end());
	for (auto it : nfa.m_Delta)
	{
		if (it.first.first == nfa.m_Initial)
			m_Delta[{m_Final + nfa.m_Initial, it.first.second}] = it.second;
		else m_Delta[it.first] = it.second;
	}
	m_Final = nfa.m_Final;
	return *this;
}

void NedeterministicFiniteAutomatonLamda::createNFA_Initial_Final_Character(std::string initial, std::string final, char character)
{
	m_Initial = initial;
	m_Final = final;
	m_Q.insert(initial);
	m_Q.insert(final);
	m_Sigma.insert(character);
	m_Delta[{initial, character}].push_back(final);
	
}

NedeterministicFiniteAutomatonLamda NedeterministicFiniteAutomatonLamda::connectAutomatonLamda(NedeterministicFiniteAutomatonLamda nfa, uint16_t contor)
{
	NedeterministicFiniteAutomatonLamda newNFA;
	newNFA.SetInitial("q" + std::to_string(contor));
	newNFA.SetFinal("q" + std::to_string(contor + 1));

	//merged m_Q
	newNFA.m_Q.insert(nfa.m_Q.begin(), nfa.m_Q.end());
	newNFA.m_Q.insert(this->m_Q.begin(), this->m_Q.end());
	//merged m_Sigma
	newNFA.m_Sigma.insert(nfa.m_Sigma.begin(), nfa.m_Sigma.end());
	newNFA.m_Sigma.insert(this->m_Sigma.begin(), this->m_Sigma.end());
	//merged m_Delta
	newNFA.m_Delta.insert(nfa.m_Delta.begin(), nfa.m_Delta.end());
	newNFA.m_Delta.insert(this->m_Delta.begin(), this->m_Delta.end());

	//Added lamda tranzitions
	newNFA.m_Delta[{newNFA.m_Initial, '#'}].push_back(this->m_Initial);
	newNFA.m_Delta[{newNFA.m_Initial, '#'}].push_back(nfa.m_Initial);
	newNFA.m_Delta[{this->m_Final, '#'}].push_back(newNFA.m_Final);
	newNFA.m_Delta[{nfa.m_Final, '#'}].push_back(newNFA.m_Final);
	return newNFA;
}

void NedeterministicFiniteAutomatonLamda::modifyTpLamdaTranzitions(int contor)
{
	std::string lastInitial = m_Initial, lastFinal = m_Final;
	this->SetInitial("q" + std::to_string(contor));
	this->SetFinal("q" + std::to_string(contor + 1));
	this->m_Delta[{m_Initial, '#'}].push_back(lastInitial);
	this->m_Delta[{lastFinal, '#'}].push_back(m_Final);
	this->m_Delta[{lastFinal, '#'}].push_back(lastInitial);
	this->m_Delta[{m_Initial, '#'}].push_back(m_Final);
}

void NedeterministicFiniteAutomatonLamda::NedeterministicToDeterministic(DeterministicFiniteAutomaton dfa)
{
	int count;

	std::vector<std::unordered_set<std::string>> lamdaClosures;
	std::unordered_map<std::unordered_set<std::string>, std::unordered_set<std::string>* > resultFromLamdaClosure;
	std::unordered_map<std::string, std::unordered_set<std::string>* > newQ;


	//std::map<std::unordered_set<std::string>, std::pair<std::string, std::unordered_set<std::string>>> map;

	std::unordered_set<std::string> set;
	set.insert(m_Initial);
	lamdaClosures.push_back(LamdaClosureForQ(m_Initial));
	resultFromLamdaClosure[set] = &lamdaClosures[0];


	dfa.AddInQ("q_0");
	dfa.SetSigma(m_Sigma);
	auto it = dfa.GetQ().begin();
	while (it != dfa.GetQ().end())
	{
		for (auto character : m_Sigma)
		{

		}

		++it;
	}


}

void NedeterministicFiniteAutomatonLamda::PrintAutomaton()
{
	std::cout << "M = ({";
	for (const auto& i : m_Q)
	{
		if (&i != &*m_Q.rbegin())
		{
			std::cout << i << ", ";
		}
		else std::cout << i << "}, {";
	}
	for (auto i : m_Sigma)
	{
		if (&i != &*m_Sigma.rbegin())
		{
			std::cout << i << ", ";
		}
		else std::cout << i << "}, delta, ";
	}
	std::cout << m_Initial << ", {";
	std::cout << m_Final << "}), where delta is:\n";
	for (auto [key, vector] : m_Delta)
	{
		auto [state, character] = key;
		for (auto i : vector)
		{
			std::cout << "(" << state << ", " << character << ") = ";
			std::cout << i << ";\n";
		}
	}
}
