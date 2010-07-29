
#include "integervalue.h"

IntegerValue::IntegerValue( int value )
: value_( value )
{
}

IntegerValue& IntegerValue::operator+=( const IntegerValue& other )
{
    // TODO: handle large integers
    value_ += other.value_;
    return *this;
}

int IntegerValue::GetIntValue() const
{
    return value_;
}

//virtual
IntegerValue* IntegerValue::Clone() const
{
    return new IntegerValue( *this );
}



