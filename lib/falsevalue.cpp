
#include "falsevalue.h"

//virtual
FalseValue* FalseValue::Clone() const
{
    return new FalseValue( *this );
}



