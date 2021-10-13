#pragma once
#include <vector>
enum TokenType
{
    INC,
    DEC,
    NEXT,
    PREV,
    READ,
    WRITE,
    LOOPSTART,
    LOOPEND
};

struct Token
{
    TokenType type;
    int count;

    Token(TokenType type, int count = 1)
    {
        this->count = count;
        this->type = type;
    }
};

std::vector<Token> Tokenise(const char* input);
std::vector<Token> Condense(std::vector<Token> tokens);
void Compile(std::vector<Token> tokens, const char* outputPath);