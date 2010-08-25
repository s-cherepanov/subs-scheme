
#ifndef PARSER_H
#define PARSER_H

#include <memory>

class ILexer;
class Value;

class Parser
{
public:
    Parser( ILexer& lexer );

    std::auto_ptr<Value> NextValue();

private:

    ILexer& lexer_;
};

#endif

