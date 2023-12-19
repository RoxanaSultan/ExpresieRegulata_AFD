#include "PolishNotation.h"

bool PolishNotation::IsOperator(const char& character)
{
    if (character == '.' || character == '|' || character == '*') return true;
    return false;
}

int PolishNotation::OperatorRank(const char& character)
{
    if (character == '(') return 0;
    if (character == '|') return 1;
    if (character == '.') return 2;
    if (character == '*') return 3;
}

std::queue<char> PolishNotation::CreatePolishNotation(std::string expression)
{
    std::stack<char> operators;
    for (int i = 0; i < expression.size(); i++)
    {
        if (isdigit(expression[i]) || isalpha(expression[i])) m_polishNotation.push(expression[i]);
        else
        {
            if (expression[i] == '(') operators.push(expression[i]);
            else if (expression[i] == ')')
            {
                while (!operators.empty() && operators.top() != '(')
                {
                    m_polishNotation.push(operators.top());
                    operators.pop();
                }
                operators.pop();
            }
            else if (IsOperator(expression[i]))
            {
                while (!operators.empty() && OperatorRank(operators.top()) >= OperatorRank(expression[i]))
                {
                    m_polishNotation.push(operators.top());
                    operators.pop();
                }
                operators.push(expression[i]);
            }
        }
    }
    while (!operators.empty())
    {
        m_polishNotation.push(operators.top());
        operators.pop();
    }
    return m_polishNotation;
}

std::queue<char> PolishNotation::GetQueuePolishNotation()
{
    return m_polishNotation;
}
