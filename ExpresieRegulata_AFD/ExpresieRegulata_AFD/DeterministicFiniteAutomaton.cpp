#include "DeterministicFiniteAutomaton.h"

DeterministicFiniteAutomaton::DeterministicFiniteAutomaton()
{}

DeterministicFiniteAutomaton::DeterministicFiniteAutomaton(const DeterministicFiniteAutomaton& other) :
    m_Q{ other.m_Q }, m_Sigma{ other.m_Sigma }, m_Delta{ other.m_Delta }, m_Initial{ other.m_Initial }, m_Finals{ other.m_Finals }
{}

bool DeterministicFiniteAutomaton::VerifyAutomaton()
{
    //Rule1 - m_Q is not empty
    if (m_Q.empty())
    {
        return false;
    }

    //Rule2 - m_Sigma is not empty
    if (m_Sigma.empty())
    {
        return false;
    }

    //Rule3 - m_Initial is in m_Q
    if (find(m_Q.begin(), m_Q.end(), m_Initial) == m_Q.end())
    {
        return false;
    }

    //Rule4 - m_F has or is equal to m_Q
    for (auto state : m_Finals)
    {
        if (find(m_Q.begin(), m_Q.end(), state) == m_Q.end())
        {
            return false;
        }
    }

    //Rule5 - the key in map is valid
    for (auto it : m_Delta)
    {
        if (find(m_Q.begin(), m_Q.end(), it.first.first) == m_Q.end())
        {
            return false;
        }
        if (find(m_Sigma.begin(), m_Sigma.end(), it.first.second) == m_Sigma.end() && it.first.second != '#')
        {
            return false;
        }
        if (find(m_Q.begin(), m_Q.end(), it.second) == m_Q.end())
        {
            return false;
        }
    }
    return true;
}

void DeterministicFiniteAutomaton::PrintAutomaton()
{
    std::ofstream file("file.out");
    if (!file.is_open())
    {
        std::cout << "The file did not open correctly!\n";
    }
    std::cout << "M = ({";
    file << "M = ({";
    for (const auto& i : m_Q)
    {
        if (&i != &*m_Q.rbegin())
        {
            std::cout << i << ", ";
            file << i << ", ";
        }
        else
        {
            std::cout << i << "}, {";
            file << i << "}, {";
        }
    }
    for (const auto& i : m_Sigma)
    {
        if (&i != &*m_Sigma.rbegin())
        {
            std::cout << i << ", ";
            file << i << ", ";
        }
        else
        {
            std::cout << i << "}, delta, ";
            file << i << "}, delta, ";
        }
    }
    std::cout << m_Initial << ", {";
    file << m_Initial << ", {";
    for (const auto& i : m_Finals)
    {
        if (&i != &*m_Finals.rbegin())
        {
            std::cout << i << ", ";
            file << i << ", ";
        }
        else
        {
            std::cout << i << "}), where delta is:\n";
            file << i << "}), where delta is:\n";
        }
    }
    if (m_Finals.empty())
    {
        std::cout << "}), where delta is:\n";
        file << "}), where delta is:\n";
    }
    for (auto [key, value] : m_Delta)
    {
        auto [state, character] = key;
        std::cout << "(" << state << ", " << character << ") = ";
        file << "(" << state << ", " << character << ") = ";
        std::cout << value << ";\n";
        file << value << ";\n";
    }
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
        currentQ = it->second;
        word.erase(word.begin(), word.begin() + 1);
    }
    if (m_Finals.find(currentQ) != m_Finals.end())
    {
        return true;
    }
    return false;
}

void DeterministicFiniteAutomaton::SetQ(std::string q)
{
    m_Q.insert(q);
}

void DeterministicFiniteAutomaton::SetSigma(std::set<char> sigma)
{
    m_Sigma.insert(sigma.begin(), sigma.end());
}

void DeterministicFiniteAutomaton::SetFinals(std::set<std::string> finals)
{
    m_Finals = finals;
}

void DeterministicFiniteAutomaton::SetDelta(std::string Key, char character, std::string Value)
{
    m_Delta[{Key, character}] = Value;
}

void DeterministicFiniteAutomaton::SetInitial(std::string state)
{
    m_Initial = state;
}

std::set<std::string> DeterministicFiniteAutomaton::GetQ()
{
    return m_Q;
}
