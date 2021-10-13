#include "bfc.h"

std::vector<Token> Condense(std::vector<Token> tokens)
{
    bool condensed = true;

    while (condensed)
    {
        condensed = false;
        for (int i = 0; i < tokens.size() - 1; i++)
        {
            if (tokens[i].type == tokens[i + 1].type && (int)tokens[i].type < 4) {
                condensed = true;
                tokens[i] = Token(tokens[i].type, tokens[i].count + tokens[i + 1].count);
                tokens.erase(tokens.begin() + (i + 1));
                break;
            }
        }
    }
    return tokens;
}