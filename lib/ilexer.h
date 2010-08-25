
#ifndef ILEXER_H
#define ILEXER_H

struct Token;

class ILexer
{
public:
    virtual Token NextToken() = 0;
};

#endif

