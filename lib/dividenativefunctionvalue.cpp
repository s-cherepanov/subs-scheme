
#include <cassert>
#include <memory>

#include "combinationvalue.h"
#include "decimalvalue.h"
#include "evaluationerror.h"
#include "integervalue.h"
#include "prettyprinter.h"
#include "value.h"

#include "dividenativefunctionvalue.h"

using namespace std;

namespace
{

const Value* get_numeric_value( CombinationValue::const_iterator& it )
{
    const Value* value = *it;
    const Value* result = dynamic_cast<const IntegerValue*>( value );
    if( !result )
    {
        result = dynamic_cast<const DecimalValue*>( value );
        if( !result )
        {
            throw EvaluationError( "Invalid argument for /: '"
                + PrettyPrinter::Print( value )
                + "' is not an integer or a decimal." );
        }
    }
    return result;
}

const DecimalValue* get_decimal_value( const Value* value )
{
    const DecimalValue* result = dynamic_cast<const DecimalValue*>( value );
    if( !result )
    {
        throw EvaluationError( "Invalid argument for /: '"
            + PrettyPrinter::Print( value )
            + "' is not an integer or a decimal." );
    }
    return result;
}

}

//virtual
std::auto_ptr<Value> DivideNativeFunctionValue::Run(
    const CombinationValue* argvalues, const Environment& environment ) const
{
    // TODO: values other than integers
    auto_ptr<Value> result;

    CombinationValue::const_iterator it = argvalues->begin();

    if( it == argvalues->end() )
    {
        throw EvaluationError( "Not enough operands for '/' - there must "
            "be at least 1." );
    }

    if( argvalues->size() > 1 )
    {
        result.reset( get_numeric_value( it )->Clone() );
        ++it;
    }
    else
    {
        result.reset( new IntegerValue( 1 ) );
    }

    for( ; it != argvalues->end(); ++it )
    {
        const Value* operand = get_numeric_value( it );

        // TODO: some kind of double dispatch?
        const IntegerValue* intop = dynamic_cast<const IntegerValue*>(
            operand );
        const IntegerValue* intres = dynamic_cast<const IntegerValue*>(
            result.get() );

        if( intres && intop )
        {
            result = *intres / *intop;
        }
        else if( intres )
        {
            result = *intres / *get_decimal_value( operand );
        }
        else if( intop )
        {
            result = *get_decimal_value( result.get() ) / *intop;
        }
        else
        {
            *get_decimal_value( result.get() ) / *get_decimal_value( operand );
        }
    }

    return result;
}


//virtual
DivideNativeFunctionValue* DivideNativeFunctionValue::Clone() const
{
    return new DivideNativeFunctionValue( *this );
}


//virtual
std::string DivideNativeFunctionValue::GetName() const
{
    return StaticName();
}

//static
const std::string& DivideNativeFunctionValue::StaticName()
{
    static const string static_name( "/" );
    return static_name;
}


