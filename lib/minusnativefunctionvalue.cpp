
#include <cassert>
#include <memory>

#include "combinationvalue.h"
#include "evaluationerror.h"
#include "integervalue.h"
#include "prettyprinter.h"
#include "value.h"

#include "minusnativefunctionvalue.h"

using namespace std;

//virtual
std::auto_ptr<Value> MinusNativeFunctionValue::Run(
    const CombinationValue* argvalues, const Environment& environment ) const
{
    // TODO: values other than integers

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
            throw EvaluationError( "Invalid argument for -: '"
                + PrettyPrinter::Print( *it ) + "' is not an integer." );
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
            throw EvaluationError( "Invalid argument for -: '"
                + PrettyPrinter::Print( *it ) + "' is not an integer." );
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


