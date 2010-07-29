
#ifndef SYMBOLVALUEMAP_H
#define SYMBOLVALUEMAP_H

#include <map>
#include <string>

class Value;

/**
 * A map of symbol names to their values, which owns the values.
 */
class SymbolValueMap : public std::map<std::string, Value*>
{
public:
    ~SymbolValueMap();
};

#endif

