
#ifndef NEWPARSER_H
#define NEWPARSER_H

#include <memory>

class NewLexer;
class Value;

class NewParser
{
public:
    NewParser( NewLexer& lexer );

    std::auto_ptr<Value> NextValue();

private:

    NewLexer& lexer_;
};

#endif

