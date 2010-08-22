
#ifndef NEWLEXER_H
#define NEWLEXER_H

#include <iostream>
#include <memory>

struct Token;

class NewLexer
{
public:
    NewLexer( std::istream& instream );

    std::auto_ptr<Token> NextToken();

private:
    std::istream& instream_;
};

#endif

