
#ifndef BRACKETMATCHER_H
#define BRACKETMATCHER_H

#include <iosfwd>

#include "newlineprocessor.h"
#include "tokenlist.h"

class Lexer;

class BracketMatcher
{
public:
    BracketMatcher( Lexer& lexer, std::ostream& out, bool print_prompt );

    ~BracketMatcher();

    /**
     * Read tokens from the stream until the last bracket is matched.
     * Then return the list of tokens found.
     */
    TokenList ReadCombination();

    bool eof() const;

private:
    Lexer& lexer_;
    NewLineProcessor newline_processor_;
};

#endif

