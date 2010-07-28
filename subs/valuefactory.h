
#ifndef VALUEFACTORY_H
#define VALUEFACTORY_H

#include <memory>

class StringTreeLeaf;
class Value;

namespace ValueFactory
{

std::auto_ptr<Value> CreateValue( const StringTreeLeaf* leaf );

};

#endif

