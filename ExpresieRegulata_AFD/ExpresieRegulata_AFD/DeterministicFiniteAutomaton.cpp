#include "DeterministicFiniteAutomaton.h"

DeterministicFiniteAutomaton::DeterministicFiniteAutomaton()
{
}

DeterministicFiniteAutomaton::DeterministicFiniteAutomaton(const DeterministicFiniteAutomaton& other)
{
}

bool DeterministicFiniteAutomaton::VerifyAutomaton()
{
	return false;
}

void DeterministicFiniteAutomaton::PrintAutomaton()
{
}

bool DeterministicFiniteAutomaton::CheckWord(std::string word)
{
    for (auto letter : word)
    {
        if (std::ranges::find(m_Sigma, letter) == m_Sigma.end())
        {
            std::cout << "The letter is not part of Sigma!\n";
            return false;
        }
    }
    std::string currentQ = m_Initial;
    while (word.size() > 0) 
    {
        auto it = m_Delta.find({ currentQ, word[0] });
        if (it == m_Delta.end())
        {
            std::cout << "Jam!\n";
            return false;
        }
        currentQ = it->second[0];
        word.erase(word.begin(), word.begin() + 1);
    }
    for (auto index : m_Finals)
    {
        if (currentQ == index)
        {
            return true;
        }
    }
    return false;
}

bool DeterministicFiniteAutomaton::IsDeterministic()
{
	return false;
}
