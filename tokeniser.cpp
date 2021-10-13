#include "bfc.h"
#include <list>

std::vector<Token> Tokenise(const char* input)
{
	std::vector<Token> tokens;
	std::list<int> countStack;
	int i = 0;
	int opened = 0;
	while (input[i] != '\0')
	{
		switch (input[i])
		{
		case '+':
			tokens.push_back(Token(TokenType::INC));
			break;
		case '-':
			tokens.push_back(Token(TokenType::DEC));
			break;
		case '>':
			tokens.push_back(Token(TokenType::NEXT));
			break;
		case '<':
			tokens.push_back(Token(TokenType::PREV));
			break;
		case ',':
			tokens.push_back(Token(TokenType::READ));
			break;
		case '.':
			tokens.push_back(Token(TokenType::WRITE));
			break;
		case '[':
			tokens.push_back(Token(TokenType::LOOPSTART, opened));
			countStack.push_front(opened);
			opened++;
			break;
		case ']':
			tokens.push_back(Token(TokenType::LOOPEND, countStack.front()));
			countStack.pop_front();
			break;
		}
		i++;
	}
	return tokens;
}