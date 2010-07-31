
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <map>
#include <string>

class Value;

/**
 * A map of symbol names to their values, which owns the values.
 */
class Environment : public std::map<std::string, Value*>
{
public:
    ~Environment();
};

#endif

