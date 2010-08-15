
#include "decimalvalue.h"
#include "integervalue.h"

using namespace std;

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


IntegerValue& IntegerValue::operator-=( const IntegerValue& other )
{
    value_ -= other.value_;
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

std::auto_ptr<Value> operator/( const IntegerValue& left,
    const IntegerValue& right )
{
    if( left.value_ % right.value_ )
    {
        return auto_ptr<Value>( new DecimalValue( static_cast<double>(
            left.value_ ) / static_cast<double>( right.value_ ) ) );
    }
    else
    {
        return auto_ptr<Value>( new IntegerValue( left.value_ / right.value_ )
            );
    }
}

