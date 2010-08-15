
#include <memory>

#include "decimalvalue.h"
#include "integervalue.h"

using namespace std;

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


std::auto_ptr<Value> operator/( const DecimalValue& left,
    const IntegerValue& right )
{
    return auto_ptr<Value>( new DecimalValue(
        left.GetDoubleValue() / static_cast<double>( right.GetIntValue() ) ) );
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

