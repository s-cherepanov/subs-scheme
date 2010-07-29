
#include <sstream>
#include <string>

#include "combinationvalue.h"
#include "integervalue.h"
#include "value.h"

#include "prettyprinter.h"

using namespace std;

namespace PrettyPrinter
{

std::string Print( const Value* value )
{
    ostringstream result;

    const IntegerValue* integervalue = dynamic_cast<const IntegerValue*>(
        value );
    if( integervalue )
    {
        result << integervalue->GetIntValue();
    }
    else
    {
        const CombinationValue* combovalue = dynamic_cast<
            const CombinationValue*>( value );

        if( combovalue )
        {
            result << "(";
            CombinationValue::const_iterator it = combovalue->begin();
            while( true )
            {
                result << Print( *it );

                ++it;

                if( it == combovalue->end() )
                {
                    break;
                }
                else
                {
                    result << " ";
                }
            }
            result << ")";
        }
    }
    // TODO other types

    return result.str();
}

}

