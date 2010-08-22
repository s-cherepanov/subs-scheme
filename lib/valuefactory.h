
#ifndef VALUEFACTORY_H
#define VALUEFACTORY_H

#include <memory>
#include <string>

class Value;

namespace ValueFactory
{

std::auto_ptr<Value> CreateValue( const std::string& token_name );

};

#endif

