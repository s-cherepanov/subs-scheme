
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

IntegerValue& IntegerValue::operator*=( const IntegerValue& other )
{
    // TODO: handle large integers
    value_ *= other.value_;
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


bool operator==( const IntegerValue& left, const IntegerValue& right )
{
    return ( left.value_ == right.value_ );
}

bool operator!=( const IntegerValue& left, const IntegerValue& right )
{
    return ( left.value_ != right.value_ );
}

