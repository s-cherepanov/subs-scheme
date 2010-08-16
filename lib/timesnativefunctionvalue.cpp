
#include <cassert>
#include <memory>

#include "combinationvalue.h"
#include "decimalvalue.h"
#include "evaluationerror.h"
#include "integervalue.h"
#include "prettyprinter.h"
#include "value.h"

#include "timesnativefunctionvalue.h"

using namespace std;

namespace
{

std::auto_ptr<Value> run_decimal( const IntegerValue* intres,
    CombinationValue::const_iterator it, const CombinationValue* argvalues )
{
    auto_ptr<DecimalValue> result( new DecimalValue( *intres ) );

    for( ; it != argvalues->end(); ++it )
    {
        const DecimalValue* decop = dynamic_cast<DecimalValue*>( *it );
        if( decop )
        {
            *result *= *decop;
        }
        else
        {
            const IntegerValue* intop = dynamic_cast<IntegerValue*>( *it );
            if( !intop )
            {
                throw EvaluationError( "Invalid argument for *: '"
                    + PrettyPrinter::Print( *it )
                    + "' is not an integer or a decimal." );
            }
            else
            {
                *result *= *intop;
            }
        }
    }

    return auto_ptr<Value>( result.release() );
}

}

//virtual
std::auto_ptr<Value> TimesNativeFunctionValue::Run(
    const CombinationValue* argvalues, const Environment& environment ) const
{
    // TODO: no need to pass in environment?

    auto_ptr<IntegerValue> result( new IntegerValue( 1 ) );

    CombinationValue::const_iterator it = argvalues->begin();

    for( ; it != argvalues->end(); ++it )
    {
        const IntegerValue* operand = dynamic_cast<IntegerValue*>( *it );
        if( !operand )
        {
            return run_decimal( result.get(), it, argvalues );
            
        }

        *result *= *operand;
    }

    return auto_ptr<Value>( result.release() );
}


//virtual
TimesNativeFunctionValue* TimesNativeFunctionValue::Clone() const
{
    return new TimesNativeFunctionValue( *this );
}


//virtual
std::string TimesNativeFunctionValue::GetName() const
{
    return StaticName();
}

//static
const std::string& TimesNativeFunctionValue::StaticName()
{
    static const string static_name( "*" );
    return static_name;
}


