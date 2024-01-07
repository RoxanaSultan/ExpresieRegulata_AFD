#include "NedeterministicFiniteAutomatonLamda.h"

std::unordered_set<std::string> NedeterministicFiniteAutomatonLamda::LamdaClosure(std::unordered_set<std::string> q)
{
	std::unordered_set<std::string> closures;
	for (auto it : q)
	{
		LamdaClosureForQ(it, closures);
		/*closures.insert(newUnordered_set.begin(), newUnordered_set.end());
		closures.insert(it);*/
	}
	closures.insert(q.begin(), q.end());
	return closures;
}

std::unordered_set<std::string> NedeterministicFiniteAutomatonLamda::LamdaClosureForQ(std::string q, std::unordered_set<std::string>& closures)
{
	//std::unordered_set<std::string> closures;
	if (m_Delta.find({ q, '#' }) != m_Delta.end())
	{
		auto myVector = m_Delta.find({ q, '#' })->second;
		for (auto it : myVector)
		{
			if(closures.find(it) == closures.end())
			{
				closures.insert(it);
				auto newSet = LamdaClosureForQ(it, closures);
				closures.insert(newSet.begin(), newSet.end());
			}
		}
		//closures.insert(myVector.begin(), myVector.end());
	}

	return closures;
}

std::unordered_set<std::string> NedeterministicFiniteAutomatonLamda::QWithCharacter(std::string q, char character)
{
	std::unordered_set<std::string> set;
	std::vector<std::string> foundVector;
	for (auto it : m_Delta)
	{
		std::string myString = it.first.first;
		if (myString.size() >= q.size() && (myString.find(q + "q") != std::string::npos || myString.compare(myString.size() - q.size(), q.size(), q) == 0 || myString == q) && it.first.second == character)
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
		if (m_Finals.find(it) != m_Finals.end())
		{
			return true;
		}
	}
	return false;
}

bool NedeterministicFiniteAutomatonLamda::isInResultsFromLamda(std::unordered_set<std::string> UnorderedsetToFind, std::unordered_map<std::unordered_set<std::string>, std::unordered_set<std::string>, UnorderedSetHash> resultFromLamdaClosure)
{
	std::set<std::string> setToFind;
	setToFind.insert(UnorderedsetToFind.begin(), UnorderedsetToFind.end());
	for (auto& [key, value] : resultFromLamdaClosure)
	{
		std::set<std::string> setToSearchIn;
		setToSearchIn.insert(key.begin(), key.end());
		if (setToFind == setToSearchIn)
			return true;
	}
	return false;
}


NedeterministicFiniteAutomatonLamda::NedeterministicFiniteAutomatonLamda()
{}

NedeterministicFiniteAutomatonLamda::NedeterministicFiniteAutomatonLamda(const NedeterministicFiniteAutomatonLamda& afn) :
	m_Q{ afn.m_Q },
	m_Sigma{ afn.m_Sigma },
	m_Delta{ afn.m_Delta },
	m_Initial{ afn.m_Initial },
	m_Finals{ afn.m_Finals }
{}

NedeterministicFiniteAutomatonLamda& NedeterministicFiniteAutomatonLamda::operator=(const NedeterministicFiniteAutomatonLamda& afn)
{
	m_Q = afn.m_Q;
	m_Sigma = afn.m_Sigma;
	m_Delta = afn.m_Delta;
	m_Initial = afn.m_Initial;
	m_Finals = afn.m_Finals;
	return *this;
}

void NedeterministicFiniteAutomatonLamda::SetInitial(const std::string& initial)
{
	m_Initial = initial;
	if (std::ranges::find(m_Q, initial) == m_Q.end())
		m_Q.insert(initial);
}

void NedeterministicFiniteAutomatonLamda::SetFinals(const std::set<std::string>& finals)
{
	m_Finals = finals;
	for (auto state : finals)
	{
		if (std::ranges::find(m_Q, state) == m_Q.end())
			m_Q.insert(state);
	}
}

std::string NedeterministicFiniteAutomatonLamda::GetInitial() const
{
	return m_Initial;
}

std::set<std::string> NedeterministicFiniteAutomatonLamda::GetFinals() const
{
	return m_Finals;
}

NedeterministicFiniteAutomatonLamda NedeterministicFiniteAutomatonLamda::Merge(NedeterministicFiniteAutomatonLamda nfa)
{
	m_Q.insert(nfa.m_Q.begin(), nfa.m_Q.end());
	m_Q.erase(std::ranges::find(m_Q, nfa.m_Initial));
	for (auto state : m_Finals)
	{
		m_Q.erase(std::ranges::find(m_Q, state));
	}
	for (auto state : m_Finals)
	{
		m_Q.insert(state + nfa.m_Initial);
	}
	m_Sigma.insert(nfa.m_Sigma.begin(), nfa.m_Sigma.end());
	for (auto it : nfa.m_Delta)
	{
		if (it.first.first == nfa.m_Initial)
		{
			for (auto state : m_Finals)
			{
				m_Delta[{state + nfa.m_Initial, it.first.second}] = it.second;
			}
		}
		else
		{
			m_Delta[it.first] = it.second;
		}

		for (int index = 0; index < it.second.size(); index++)
		{
			for (auto& state : m_Finals)
			{
				if (it.second[index] == nfa.m_Initial)
					it.second[index] = state + nfa.m_Initial;
			}
		}
	}
	for (auto& it : m_Delta)
	{
		if (it.first.first == *m_Finals.begin())
		{
			for (auto state : m_Finals)
			{
				m_Delta[{state + nfa.m_Initial, it.first.second}].assign(it.second.begin(), it.second.end());
			}
		}
		for (int index = 0 ;index < it.second.size(); index ++)
		{
			for(auto& state: m_Finals)
			{
				if (it.second[index] == state)
					it.second[index] = state + nfa.m_Initial;
			}
		}
	}
	m_Finals = nfa.m_Finals;
	return *this;
}

void NedeterministicFiniteAutomatonLamda::createNFA_Initial_Final_Character(std::string initial, std::set<std::string> finals, char character)
{
	m_Initial = initial;
	m_Finals = finals;
	m_Q.insert(initial);
	for (auto state : finals)
	{
		m_Q.insert(state);
	}
	m_Sigma.insert(character);
	for (auto state : finals)
	{
		m_Delta[{initial, character}].push_back(state);
	}
}

NedeterministicFiniteAutomatonLamda NedeterministicFiniteAutomatonLamda::connectAutomatonLamda(NedeterministicFiniteAutomatonLamda nfa, uint16_t contor)
{
	NedeterministicFiniteAutomatonLamda newNFA;
	newNFA.SetInitial("q" + std::to_string(contor));
	newNFA.SetFinals({ "q" + std::to_string(contor + 1) });

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
	for (auto state : m_Finals)
	{
		for (auto stateNewNFA : newNFA.m_Finals)
		{
			newNFA.m_Delta[{state, '#'}].push_back(stateNewNFA);
		}
	}
	for (auto state : nfa.m_Finals)
	{
		for (auto stateNewNFA : newNFA.m_Finals)
		{
			newNFA.m_Delta[{state, '#'}].push_back(stateNewNFA);
		}
	}
	return newNFA;
}

void NedeterministicFiniteAutomatonLamda::modifyToLamdaTranzitions(int contor)
{
	std::string lastInitial = m_Initial;
	std::set<std::string> lastFinal = m_Finals;
	this->SetInitial("q" + std::to_string(contor));
	this->SetFinals({ "q" + std::to_string(contor + 1) });
	this->m_Delta[{m_Initial, '#'}].push_back(lastInitial);
	for (auto state : lastFinal)
	{
		for (auto stateFinal : m_Finals)
		{
			this->m_Delta[{state, '#'}].push_back(stateFinal);
		}
	}
	for (auto state : lastFinal)
	{
		this->m_Delta[{state, '#'}].push_back(lastInitial);
	}
	for (auto state : m_Finals)
	{
		this->m_Delta[{m_Initial, '#'}].push_back(state);
	}
}


void NedeterministicFiniteAutomatonLamda::NedeterministicToDeterministic(DeterministicFiniteAutomaton& dfa)
{
	std::unordered_map<std::unordered_set<std::string>, std::unordered_set<std::string>, UnorderedSetHash> resultFromLamdaClosure;
	std::unordered_map<std::string, std::unordered_set<std::string> > newQ;

	std::unordered_set<std::string> lamdaClosures, states;
	int index = 0;
	LamdaClosureForQ(m_Initial, lamdaClosures);
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

	auto aux = dfa.GetQ();
	std::vector<std::string> Q(aux.begin(), aux.end());

	int ind = 0;

	while (ind < Q.size())
	{
		const std::string currentState = Q[ind];
		for (auto character : m_Sigma)
		{
			for (auto status : newQ[currentState])
			{
				std::unordered_set<std::string> aux = QWithCharacter(status, character);
				states.insert(aux.begin(), aux.end());
			}

			lamdaClosures = LamdaClosure(states);
			if (states.size())
			{
				if (!isInResultsFromLamda(states, resultFromLamdaClosure))
				{
					resultFromLamdaClosure[states] = lamdaClosures;
					newQ["q_" + std::to_string(index)] = lamdaClosures;
					dfa.SetQ("q_" + std::to_string(index));
					dfa.SetDelta(currentState, character, "q_" + std::to_string(index));
					index++;
				}
				else
				{
					dfa.SetDelta(currentState, character, FindCurrentStateWithLamdaClosures(newQ, lamdaClosures));
				}
			}

			lamdaClosures.clear();
			states.clear();
		}
		auto aux = dfa.GetQ();
		Q.assign(aux.begin(), aux.end());
		ind++;
	}

	for (auto [state, closures] : newQ)
	{
		if (IsFinal(closures))
		{
			dfa.SetFinals({ state });
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
	for (const auto& i : m_Finals)
	{
		if (&i != &*m_Finals.rbegin())
		{
			std::cout << i << ", ";
		}
		else std::cout << i << "}), where delta is:\n";
	}
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

std::string FindCurrentStateWithLamdaClosures(std::unordered_map<std::string, std::unordered_set<std::string>> myMap, std::unordered_set<std::string> setToFind)
{
	for (auto& [string, set] : myMap)
	{
		if (set == setToFind)
			return string;
	}
	return std::string();
}
