
#include <memory>

#include "decimalvalue.h"
#include "integervalue.h"

using namespace std;

//explicit
DecimalValue::DecimalValue( double value )
: value_( value )
{
}

//explicit
DecimalValue::DecimalValue( const IntegerValue& integer_value )
: value_( static_cast<double>( integer_value.GetIntValue() ) )
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

DecimalValue& DecimalValue::operator+=( const IntegerValue& other )
{
    value_ += static_cast<double>( other.GetIntValue() );
    return *this;
}

DecimalValue& DecimalValue::operator+=( const DecimalValue& other )
{
    value_ += other.value_;
    return *this;
}


DecimalValue& DecimalValue::operator*=( const IntegerValue& other )
{
    value_ *= static_cast<double>( other.GetIntValue() );
    return *this;
}

DecimalValue& DecimalValue::operator*=( const DecimalValue& other )
{
    value_ *= other.value_;
    return *this;
}

DecimalValue& DecimalValue::operator/=( const IntegerValue& other )
{
    value_ /= static_cast<double>( other.GetIntValue() );
    return *this;
}

DecimalValue& DecimalValue::operator/=( const DecimalValue& other )
{
    value_ /= other.value_;
    return *this;
}


std::auto_ptr<Value> operator/( const IntegerValue& left,
    const DecimalValue& right )
{
    return auto_ptr<Value>( new DecimalValue(
        static_cast<double>( left.GetIntValue() ) / right.GetDoubleValue() ) );
}

std::auto_ptr<Value> operator/( const DecimalValue& left,
    const DecimalValue& right )
{
    return auto_ptr<Value>( new DecimalValue( left.GetDoubleValue() /
        right.GetDoubleValue() ) );
}

