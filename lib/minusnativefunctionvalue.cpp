
#include <cassert>
#include <memory>

#include "combinationvalue.h"
#include "decimalvalue.h"
#include "evaluationerror.h"
#include "integervalue.h"
#include "prettyprinter.h"
#include "value.h"

#include "minusnativefunctionvalue.h"

using namespace std;

namespace
{

void run_decimal( auto_ptr<DecimalValue>& result,
    CombinationValue::const_iterator it, const CombinationValue* argvalues )
{
    for( ; it != argvalues->end(); ++it )
    {
        const DecimalValue* decop = dynamic_cast<DecimalValue*>( *it );
        if( decop )
        {
            *result -= *decop;
        }
        else
        {
            const IntegerValue* intop = dynamic_cast<IntegerValue*>( *it );
            if( !intop )
            {
                throw EvaluationError( "Invalid argument for -: '"
                    + PrettyPrinter::Print( *it )
                    + "' is not an integer or a decimal." );
            }
            else
            {
                *result -= *intop;
            }
        }
    }
}

std::auto_ptr<Value> run_decimal_from_start(
    CombinationValue::const_iterator it, const CombinationValue* argvalues )
{
    const DecimalValue* decres = dynamic_cast<const DecimalValue*>( *it );

    if( !decres )
    {
        throw EvaluationError( "Invalid argument for -: '"
            + PrettyPrinter::Print( *it )
            + "' is not an integer or a decimal." );
    }

    auto_ptr<DecimalValue> result( new DecimalValue( *decres ) );
    ++it;

    run_decimal( result, it, argvalues );

    return auto_ptr<Value>( result.release() );
}

std::auto_ptr<Value> run_decimal_from_int( const IntegerValue* intres,
    CombinationValue::const_iterator it, const CombinationValue* argvalues )
{
    auto_ptr<DecimalValue> result( new DecimalValue( *intres ) );

    run_decimal( result, it, argvalues );

    return auto_ptr<Value>( result.release() );
}

}

//virtual
std::auto_ptr<Value> MinusNativeFunctionValue::Run(
    const CombinationValue* argvalues, const Environment& environment ) const
{
    CombinationValue::const_iterator it = argvalues->begin();

    if( it == argvalues->end() )
    {
        throw EvaluationError( "Not enough operands for '-' - there must "
            "be at least 1." );
    }

    int initial_value = 0;

    if( argvalues->size() > 1 )
    {
        const IntegerValue* operand = dynamic_cast<IntegerValue*>( *it );
        if( !operand )
        {
            // Switch into decimal mode and return immediately
            return run_decimal_from_start( it, argvalues );
        }

        initial_value = operand->GetIntValue();

        ++it;
        assert( it != argvalues->end() ); // We have just checked there are more
    }

    auto_ptr<IntegerValue> result( new IntegerValue( initial_value ) );

    for( ; it != argvalues->end(); ++it )
    {
        const IntegerValue* operand = dynamic_cast<IntegerValue*>( *it );
        if( !operand )
        {
            // Switch into decimal mode and return immediately
            return run_decimal_from_int( result.get(), it, argvalues );
        }

        *result -= *operand;
    }

    return auto_ptr<Value>( result.release() );
}


//virtual
MinusNativeFunctionValue* MinusNativeFunctionValue::Clone() const
{
    return new MinusNativeFunctionValue( *this );
}


//virtual
std::string MinusNativeFunctionValue::GetName() const
{
    return StaticName();
}

//static
const std::string& MinusNativeFunctionValue::StaticName()
{
    static const string static_name( "-" );
    return static_name;
}


