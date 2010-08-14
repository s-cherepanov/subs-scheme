
#include <cassert>
#include <memory>

#include "combinationvalue.h"
#include "evaluationerror.h"
#include "integervalue.h"
#include "prettyprinter.h"
#include "value.h"

#include "plusprocedurevalue.h"

using namespace std;

//virtual
std::auto_ptr<Value> PlusProcedureValue::Run(
    const CombinationValue* argvalues, const Environment& environment ) const
{
    // TODO: values other than integers

    auto_ptr<IntegerValue> result( new IntegerValue( 0 ) );

    CombinationValue::const_iterator it = argvalues->begin();

    for( ; it != argvalues->end(); ++it )
    {
        const IntegerValue* operand = dynamic_cast<IntegerValue*>( *it );
        if( !operand )
        {
            // TODO: remove need for PrettyPrinter scattered all over the code
            throw EvaluationError( "Invalid argument for +: '"
                + PrettyPrinter::Print( *it ) + "' is not an integer." );
        }

        *result += *operand;
    }

    return auto_ptr<Value>( result.release() );
}


//virtual
PlusProcedureValue* PlusProcedureValue::Clone() const
{
    return new PlusProcedureValue( *this );
}


//virtual
std::string PlusProcedureValue::GetName() const
{
    return StaticName();
}

//static
const std::string& PlusProcedureValue::StaticName()
{
    static const string static_name( "+" );
    return static_name;
}


