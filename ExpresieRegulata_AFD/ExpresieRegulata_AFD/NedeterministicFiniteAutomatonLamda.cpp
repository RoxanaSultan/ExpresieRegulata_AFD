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
	if (m_Delta.find({ q, '#' }) != m_Delta.end())
	{
		auto myVector = m_Delta.find({ q, '#' })->second;
		closures.insert(myVector.begin(), myVector.end());
		for (auto it : myVector)
		{
			auto newSet = LamdaClosureForQ(it);
			closures.insert(newSet.begin(), newSet.end());
		}
	}
	
	return closures;
}

std::unordered_set<std::string> NedeterministicFiniteAutomatonLamda::QWithCharacter(std::string q, char character)
{
	std::unordered_set<std::string> set;
	std::vector<std::string> foundVector;
	for (auto it : m_Delta)
	{
		if (it.first.first.find(q) != std::string::npos && it.first.second == character)
		{
			foundVector = it.second;
			break;
		}
	}
	if (foundVector.size())
	{
		set.insert(foundVector.begin(), foundVector.end());
	}
	return set;
}

bool NedeterministicFiniteAutomatonLamda::IsFinal(std::unordered_set<std::string> closures)
{
	for (auto it : closures)
	{
		if (it == m_Final)
		{
			return true;
		}
	}
	return false;
}

NedeterministicFiniteAutomatonLamda::NedeterministicFiniteAutomatonLamda()
{
	//m_Sigma.insert('#');
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
		{
			m_Delta[{m_Final + nfa.m_Initial, it.first.second}] = it.second;
		}
		else
		{
			m_Delta[it.first] = it.second;
		}
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

void NedeterministicFiniteAutomatonLamda::NedeterministicToDeterministic(DeterministicFiniteAutomaton& dfa)
{
	std::unordered_map<std::unordered_set<std::string>, std::unordered_set<std::string>, UnorderedSetHash> resultFromLamdaClosure;
	std::unordered_map<std::string, std::unordered_set<std::string> > newQ;
	
	std::unordered_set<std::string> lamdaClosures, states;
	int index = 0;
		lamdaClosures = LamdaClosureForQ(m_Initial);
			lamdaClosures.insert(m_Initial);
			states.insert(m_Initial);
			resultFromLamdaClosure[states] = lamdaClosures;
			newQ["q_" + std::to_string(index)] = lamdaClosures;

			lamdaClosures.clear();
			states.clear();
			dfa.SetInitial("q_" + std::to_string(index));

			index++;

	dfa.SetQ("q_0");
	dfa.SetSigma(m_Sigma);

	auto Q = dfa.GetQ();
	auto it = Q.begin();
	auto finalIterator = Q.end();
	//de modificat Q in vector pentru a parcurge vectorul cu un while. 
	//eroare pentru ca parcurgem si modificam Q in acelasi timp

	/*while(it != finalIterator)
	{
		const std::string currentState = *it;
		for (auto character : m_Sigma)
		{ 
			for (auto status : newQ[currentState])
			{
				std::unordered_set<std::string> aux = QWithCharacter(status, character);
				states.insert(aux.begin(), aux.end());
			}
			
			lamdaClosures = LamdaClosure(states);

			if (resultFromLamdaClosure.find(states) == resultFromLamdaClosure.end())
			{
				resultFromLamdaClosure[states] = lamdaClosures;
				newQ["q_" + std::to_string(index)] = lamdaClosures;
				dfa.SetQ("q_" + std::to_string(index));
				dfa.SetDelta(index, character);
				index++;
			}

			lamdaClosures.clear();
			states.clear();
		}
		Q = dfa.GetQ();
		finalIterator = Q.end();
		it++;
	}*/

	for (auto [state, closures] : newQ)
	{
		if (IsFinal(closures))
		{
			dfa.SetFinal(state);
		}
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
	for (const auto& i : m_Sigma)
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
