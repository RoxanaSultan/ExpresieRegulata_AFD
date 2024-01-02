#include "DeterministicFiniteAutomaton.h"
    
DeterministicFiniteAutomaton::DeterministicFiniteAutomaton()
{
    //m_Sigma.insert('#');
}

DeterministicFiniteAutomaton::DeterministicFiniteAutomaton(const DeterministicFiniteAutomaton& other) :
    m_Q{ other.m_Q }, m_Sigma{ other.m_Sigma }, m_Delta{ other.m_Delta }, m_Initial{ other.m_Initial }, m_Final{ other.m_Final }
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
    //for (auto status : m_Finals)
    //{
        if (find(m_Q.begin(), m_Q.end(), m_Final) == m_Q.end())
        {
            return false;
        }
    //}

    //Rule5 - the key in map is valid
    for (auto it : m_Delta)
    {
        if (find(m_Q.begin(), m_Q.end(), it.first.first) == m_Q.end())
        {
            return false;
        }
        if (find(m_Sigma.begin(), m_Sigma.end(), it.first.second) == m_Sigma.end())
        {
            return false;
        }
        for (auto i : it.second)
        {
            if (find(m_Q.begin(), m_Q.end(), i) == m_Q.end())
            {
                return false;
            }
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
    std::cout << m_Final << "}), where delta is:\n";
    file << m_Final << "}), where delta is:\n";
    for (auto [key, vector] : m_Delta)
    {
        auto [state, character] = key;
        for (auto i : vector)
        {
            std::cout << "(" << state << ", " << character << ") = ";
            file << "(" << state << ", " << character << ") = ";
            std::cout << i << ";\n";
            file << i << ";\n";
        }
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
        currentQ = *it->second.begin();
        word.erase(word.begin(), word.begin() + 1);
    }
    //for (auto index : m_Finals)
    //{
        if (currentQ == m_Final)
        {
            return true;
        }
    //}
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

void DeterministicFiniteAutomaton::SetFinal(std::string state)
{
    m_Final = state;
}

void DeterministicFiniteAutomaton::SetDelta(int index, char character)
{
    m_Delta[{"q_" + std::to_string(index - 1), character}].push_back("q_" + std::to_string(index));
}

void DeterministicFiniteAutomaton::SetInitial(std::string state)
{
    m_Initial = state;
}

std::set<std::string> DeterministicFiniteAutomaton::GetQ()
{
    return m_Q;
}

bool DeterministicFiniteAutomaton::IsDeterministic()
{
    for (const auto& [key, value] : m_Delta)
    {
        if (value.size() != 1)
        {
            return false;
        }
    }
    return true;
}
