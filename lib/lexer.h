
#ifndef LEXER_H
#define LEXER_H

#include <iostream>

struct Token;

class Lexer
{
public:
    Lexer( std::istream& instream );

    Token NextToken();

private:
    std::istream& instream_;
    char spill_char_;
};

#endif

