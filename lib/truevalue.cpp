
#include "truevalue.h"

//virtual
TrueValue* TrueValue::Clone() const
{
    return new TrueValue( *this );
}



