
#include <string>

#include "symbolvalue.h"

SymbolValue::SymbolValue( std::string symbol )
:symbol_( symbol )
{
}

const std::string& SymbolValue::GetStringValue() const
{
    return symbol_;
}

//virtual
SymbolValue* SymbolValue::Clone() const
{
    return new SymbolValue( *this );
}


