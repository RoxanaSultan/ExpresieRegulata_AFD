#include <iostream>
#include <fstream>
#include "RegularExpression.h"
#include "NedeterministicFiniteAutomatonLamda.h"

void PrintMenu()
{
	std::cout << "\nChoose one option:\n";
	std::cout << "(a) display the deterministic finite automaton on screen and in file.\n";
	std::cout << "(b) display the regular expression.\n";
	std::cout << "(c) verify a word in automaton.\n\n";
}

int main()
{
	std::ifstream file("file.in");
	if (!file.is_open())
	{
		std::cout << "The file did not open correctly!\n";
	}
	RegularExpression myExpression;
	file >> myExpression;
	if (myExpression.IsValid())
	{
		std::cout << "Valid expression!\n";

		NedeterministicFiniteAutomatonLamda NFA = myExpression.CreateNFA();
		DeterministicFiniteAutomaton DFA;
		NFA.NedeterministicToDeterministic(DFA);

		char menu;
		int number;

		std::string word;
		bool moreOptions = true;

		//Start loop for menu
		while (moreOptions)
		{
			PrintMenu();

			//Menu start
			std::cin >> menu;
			switch (menu)
			{
			case 'a':
				DFA.PrintAutomaton();
				break;
			case 'b':
				break;
			case 'c':
				std::cout << "The word to be verified is: ";
				std::cin >> word;
				if (DFA.CheckWord(word))
				{
					std::cout << "\nThis word is accepted by the automaton.\n";
				}
				else
				{
					std::cout << "\nThis word is not accepted by the automaton.\n";
				}
				break;
			default:
				std::cout << "Invalid option!";
				break;
			}

			std::cout << "\nDo you want to choose another option?\n";
			std::string option;
			std::cin >> option;
			if (option == "no")
			{
				moreOptions = false;
			}
			std::cout << "\n";
		}
	}
	
	file.close();
}