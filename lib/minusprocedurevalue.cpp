
#include <cassert>
#include <memory>

#include "combinationvalue.h"
#include "evaluationerror.h"
#include "integervalue.h"
#include "prettyprinter.h"
#include "value.h"

#include "minusprocedurevalue.h"

using namespace std;

//virtual
std::auto_ptr<Value> MinusProcedureValue::Run( Evaluator* ev,
    const CombinationValue* combo, Environment& environment )
{
    // TODO: values other than integers

    CombinationValue::const_iterator it = combo->begin();
    assert( it != combo->end() ); // We don't get here unless there
                                 // is an operator
    ++it; // Skip the operator, move on to the operands

    if( it == combo->end() )
    {
        throw EvaluationError( "Not enough operands for '-' - there must "
            "be at least 1." );
    }

    int initial_value = 0;

    if( combo->size() > 2 )
    {
        const IntegerValue* operand = dynamic_cast<IntegerValue*>( *it );
        if( !operand )
        {
            throw EvaluationError( "Invalid argument for -: '"
                + PrettyPrinter::Print( *it ) + "' is not an integer." );
        }

        initial_value = operand->GetIntValue();

        ++it;
        assert( it != combo->end() ); // We have just checked there are more
    }

    auto_ptr<IntegerValue> result( new IntegerValue( initial_value ) );

    for( ; it != combo->end(); ++it )
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
MinusProcedureValue* MinusProcedureValue::Clone() const
{
    return new MinusProcedureValue( *this );
}


//virtual
std::string MinusProcedureValue::GetName() const
{
    return StaticName();
}

//static
const std::string& MinusProcedureValue::StaticName()
{
    static const string static_name( "-" );
    return static_name;
}


