
#ifndef LEXER_H
#define LEXER_H

#include <memory>
#include <string>

class StringTree;

class Lexer
{
public:
    std::auto_ptr<StringTree> Lex( const std::string& codestring ) const;
};

#endif

