
#include <cassert>
#include <memory>

#include "combinationvalue.h"
#include "evaluationerror.h"
#include "integervalue.h"
#include "prettyprinter.h"
#include "value.h"

#include "dividenativefunctionvalue.h"

using namespace std;

namespace
{

const IntegerValue* get_integer_value( CombinationValue::const_iterator& it )
{
    const IntegerValue* result = dynamic_cast<IntegerValue*>( *it );
    if( !result )
    {
        throw EvaluationError( "Invalid argument for /: '"
            + PrettyPrinter::Print( *it ) + "' is not an integer." );
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
        result.reset( get_integer_value( it )->Clone() );
        ++it;
    }
    else
    {
        result.reset( new IntegerValue( 1 ) );
    }

    for( ; it != argvalues->end(); ++it )
    {
        const IntegerValue* operand = get_integer_value( it );

        IntegerValue* intres = dynamic_cast<IntegerValue*>( result.get() );
        if( intres )
        {
            result = *intres / *operand;
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


