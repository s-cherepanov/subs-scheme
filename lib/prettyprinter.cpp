
#include <sstream>
#include <string>

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
    // TODO other types

    return result.str();
}

}

