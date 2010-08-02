
#include <cassert>
#include <memory>

#include "combinationvalue.h"
#include "evaluationerror.h"
#include "falsevalue.h"
#include "integervalue.h"
#include "prettyprinter.h"
#include "truevalue.h"
#include "value.h"

#include "equalsprocedurevalue.h"

using namespace std;

//virtual
std::auto_ptr<Value> EqualsProcedureValue::Run( Evaluator* ev,
    const CombinationValue* combo, Environment& environment )
{
    // TODO: values other than integers

    CombinationValue::const_iterator it = combo->begin();
    assert( it != combo->end() ); // We don't get here unless there
                                 // is an operator
    ++it; // Skip the operator, move on to the operands

    if( it == combo->end() )
    {
        throw EvaluationError( "Not enough operands to =: there should "
            "be at least 2." );
    }

    const IntegerValue* first = dynamic_cast<const IntegerValue*>( *it );

    if( !first )
    {
        throw EvaluationError( "Wrong operand type for =: an integer was "
            "expected, but '" + PrettyPrinter::Print( *it )
            + "' is not an integer." );
    }

    ++it;

    if( it == combo->end() )
    {
        throw EvaluationError( "Not enough operands to =: there should "
            "be at least 2." );
    }

    for( ; it != combo->end(); ++it )
    {
        const IntegerValue* operand = dynamic_cast<IntegerValue*>( *it );
        if( !operand )
        {
            // TODO: consistent error messages for all builtins
            throw EvaluationError( "Wrong operand type for =: an integer was "
                " expected, but '" + PrettyPrinter::Print( *it )
                + "' is not an integer." );
        }

        if( *first != *operand )
        {
            return auto_ptr<Value>( new FalseValue );
        }
    }

    return auto_ptr<Value>( new TrueValue );
}


//virtual
EqualsProcedureValue* EqualsProcedureValue::Clone() const
{
    return new EqualsProcedureValue( *this );
}


//virtual
std::string EqualsProcedureValue::GetName() const
{
    return "=";
}


