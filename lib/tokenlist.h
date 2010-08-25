
#ifndef TOKENLIST_H
#define TOKENLIST_H

#include <queue>

#include "ilexer.h"
#include "token.h"

class TokenList : public ILexer
{
public:
    void AddToken( const Token& token );
    virtual Token NextToken();

    // public for test
    bool empty() const;

private:
    std::queue<Token> list_;
};

#endif

