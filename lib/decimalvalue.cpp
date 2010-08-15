
#include "decimalvalue.h"

DecimalValue::DecimalValue( double value )
: value_( value )
{
}


double DecimalValue::GetDoubleValue() const
{
    return value_;
}

//virtual
DecimalValue* DecimalValue::Clone() const
{
    return new DecimalValue( *this );
}


