
#ifndef LEXER_H
#define LEXER_H

#include <iostream>

#include "ilexer.h"

class NewLineProcessor;
struct Token;

class Lexer : public ILexer
{
public:
    Lexer( std::istream& instream );

    virtual Token NextToken();

    void SkipWhitespaceToNewline();

    void SetNewLineProcessor( NewLineProcessor* newline_processor );

    bool EndedWithNewLine() const;

    bool eof() const;
private:
    std::istream& instream_;
    char spill_char_;
    bool ended_with_newline_;

    unsigned int column_;
    NewLineProcessor* newline_processor_;
};

#endif

