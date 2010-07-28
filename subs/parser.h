
#ifndef PARSER_H
#define PARSER_H

#include <memory>

class StringTree;
class Value;

class Parser
{
public:
    std::auto_ptr<Value> parse( const StringTree& stringtree ) const;
};

#endif

