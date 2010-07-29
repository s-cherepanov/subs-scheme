
#ifndef PRETTYPRINTER_H
#define PRETTYPRINTER_H

#include <iosfwd>
#include <string>

class Value;

namespace PrettyPrinter
{

std::string Print( const Value* value );
void Print( const Value* value, std::ostream& result );

}

#endif

