
#ifndef NEWLEXER_H
#define NEWLEXER_H

#include <iostream>

struct Token;

class NewLexer
{
public:
    NewLexer( std::istream& instream );

    Token NextToken();

private:
    std::istream& instream_;
    char spill_char_;
};

#endif

