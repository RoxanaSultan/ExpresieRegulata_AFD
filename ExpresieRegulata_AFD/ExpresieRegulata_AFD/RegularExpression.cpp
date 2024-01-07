#include "RegularExpression.h"

std::istream& operator>>(std::istream& in, RegularExpression& expression)
{
    in >> expression.m_word;
    expression.ExpressionToPolishNotation();
    return in;
}

bool RegularExpression::IsValid()
{
    std::stack<char> auxiliary;
    char previous = '\0';
    if (m_word.empty()) return false;
    for (int i = 0; i < m_word.size(); i++)
    {
        if (!isdigit(m_word[i]) && !IsOperator(m_word[i]) && m_word[i] != ' ' && m_word[i] != '	' && m_word[i] != '(' && m_word[i] != ')' && !isalpha(m_word[i])) return false;
        if (IsOperator(m_word[0]) || (IsOperator(m_word[m_word.size() - 1]) && m_word[m_word.size() - 1] != '*')) return false;
        if (IsOperator(m_word[i]) && IsOperator(previous) && previous != '*') return false;
        if (isdigit(m_word[i]) && isdigit(previous)) return false;
        if (m_word[i] == '(' || IsOperator(m_word[i])) auxiliary.push(m_word[i]);
        if (m_word[i] == ')')
        {
            if (!auxiliary.empty())
                if (IsOperator(auxiliary.top()))
                {
                    while (IsOperator(auxiliary.top()))
                        auxiliary.pop();
                    if (auxiliary.top() == '(') auxiliary.pop();
                    else return false;
                }
                else return false;
            else return false;

        }
        previous = m_word[i];
    }
    while (!auxiliary.empty())
    {
        if (auxiliary.top() == '(' || auxiliary.top() == ')') return false;
        auxiliary.pop();
    }
    return true;
}

std::string RegularExpression::GetWord()
{
    return m_word;
}

bool RegularExpression::IsOperator(const char& character)
{
    if (character == '.' || character == '|' || character == '*')
    {
        return true;
    }
    return false;
}

RegularExpression::RegularExpression(const RegularExpression& expression) : m_word{expression.m_word}, m_PolishNotation{expression.m_PolishNotation}
{}

RegularExpression& RegularExpression::operator=(const RegularExpression& expression)
{
    m_word = expression.m_word;
    m_PolishNotation = expression.m_PolishNotation;
    return *this;
}

void RegularExpression::ExpressionToPolishNotation()
{
    m_PolishNotation.CreatePolishNotation(m_word);
}

NedeterministicFiniteAutomatonLamda RegularExpression::CreateNFA()
{
    std::stack<NedeterministicFiniteAutomatonLamda> Stack;
    uint16_t index = 0, contor = 0;
    std::queue<char> Queue = m_PolishNotation.GetQueuePolishNotation();
    char character;
    std::string operators("|.*");
    while (!Queue.empty())
    {
        character = Queue.front();
        Queue.pop();
        if (std::ranges::find(operators, character) == operators.end())
        {
            NedeterministicFiniteAutomatonLamda nfa;
            nfa.createNFA_Initial_Final_Character("q" + std::to_string(contor), { "q" + std::to_string(contor + 1) }, character);
            Stack.push(nfa);
            contor += 2;
        }
        else
            if (character == '.')
            {
                NedeterministicFiniteAutomatonLamda A(Stack.top());
                Stack.pop();
                NedeterministicFiniteAutomatonLamda B(Stack.top());
                Stack.pop();
                B.Merge(A);
                Stack.push(B);
            }
            else
                if (character == '|')
                {
                    NedeterministicFiniteAutomatonLamda A(Stack.top());
                    Stack.pop();
                    NedeterministicFiniteAutomatonLamda B(Stack.top());
                    Stack.pop();
                    NedeterministicFiniteAutomatonLamda C = A.connectAutomatonLamda(B, contor);
                    Stack.push(C);
                    contor += 2;
                }
                else
                    if (character == '*')
                    {
                        Stack.top().modifyToLamdaTranzitions(contor);
                        contor += 2;
                    }
        index++;
    }
    return Stack.top();
}
