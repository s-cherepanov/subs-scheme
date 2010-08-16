
#include <cassert>
#include <memory>

#include "combinationvalue.h"
#include "decimalvalue.h"
#include "evaluationerror.h"
#include "integervalue.h"
#include "prettyprinter.h"
#include "value.h"

#include "plusnativefunctionvalue.h"

using namespace std;

namespace
{

std::auto_ptr<Value> run_decimal( const IntegerValue* intres,
    CombinationValue::const_iterator it, const CombinationValue* argvalues )
{
    std::auto_ptr<DecimalValue> result( new DecimalValue( *intres ) );

    for( ; it != argvalues->end(); ++it )
    {
        
        const DecimalValue* decop = dynamic_cast<DecimalValue*>( *it );
        if( decop )
        {
            *result += *decop;
        }
        else
        {
            const IntegerValue* intop = dynamic_cast<IntegerValue*>( *it );

            if( !intop )
            {
                // TODO: remove need for PrettyPrinter scattered everywhere
                throw EvaluationError( "Invalid argument for +: '"
                    + PrettyPrinter::Print( *it )
                    + "' is not an integer or a decimal." );
            }

            *result += *intop;
        }
    }

    return auto_ptr<Value>( result.release() );
}

}

//virtual
std::auto_ptr<Value> PlusNativeFunctionValue::Run(
    const CombinationValue* argvalues, const Environment& environment ) const
{
    auto_ptr<IntegerValue> result( new IntegerValue( 0 ) );

    CombinationValue::const_iterator it = argvalues->begin();

    for( ; it != argvalues->end(); ++it )
    {
        const IntegerValue* operand = dynamic_cast<IntegerValue*>( *it );
        if( !operand )
        {
            // As soon as we find a non-integer, continue the calculation
            // using decimals and return immediately
            return run_decimal( result.get(), it, argvalues );
        }

        *result += *operand;
    }

    return auto_ptr<Value>( result.release() );
}


//virtual
PlusNativeFunctionValue* PlusNativeFunctionValue::Clone() const
{
    return new PlusNativeFunctionValue( *this );
}


//virtual
std::string PlusNativeFunctionValue::GetName() const
{
    return StaticName();
}

//static
const std::string& PlusNativeFunctionValue::StaticName()
{
    static const string static_name( "+" );
    return static_name;
}


