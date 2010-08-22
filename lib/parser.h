
#ifndef PARSER_H
#define PARSER_H

#include <memory>

class Lexer;
class Value;

class Parser
{
public:
    Parser( Lexer& lexer );

    std::auto_ptr<Value> NextValue();

private:

    Lexer& lexer_;
};

#endif

