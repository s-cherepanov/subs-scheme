
#include <cassert>
#include <memory>

#include "combinationvalue.h"
#include "evaluationerror.h"
#include "integervalue.h"
#include "prettyprinter.h"
#include "value.h"

#include "timesprocedurevalue.h"

using namespace std;

//virtual
std::auto_ptr<Value> TimesProcedureValue::Run( Evaluator* ev,
    const CombinationValue* combo, Environment& environment )
{
    // TODO: values other than integers

    auto_ptr<IntegerValue> result( new IntegerValue( 1 ) );

    CombinationValue::const_iterator it = combo->begin();
    assert( it != combo->end() ); // We don't get here unless there
                                 // is an operator
    ++it; // Skip the operator, move on to the operands

    for( ; it != combo->end(); ++it )
    {
        const IntegerValue* operand = dynamic_cast<IntegerValue*>( *it );
        if( !operand )
        {
            // TODO: remove need for PrettyPrinter scattered all over the code
            throw EvaluationError( "Invalid argument for *: '"
                + PrettyPrinter::Print( *it ) + "' is not an integer." );
        }

        *result *= *operand;
    }

    return auto_ptr<Value>( result.release() );
}


//virtual
TimesProcedureValue* TimesProcedureValue::Clone() const
{
    return new TimesProcedureValue( *this );
}


//virtual
std::string TimesProcedureValue::GetName() const
{
    return StaticName();
}

//static
const std::string& TimesProcedureValue::StaticName()
{
    static const string static_name( "*" );
    return static_name;
}

