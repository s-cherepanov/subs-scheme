
#include <memory>
#include <sstream>
#include <string>

#include "decimalvalue.h"
#include "integervalue.h"
#include "stringtreeleaf.h"
#include "symbolvalue.h"
#include "value.h"

#include "valuefactory.h"

using namespace std;

namespace
{

enum ENumericType
{
      nonnumeric
    , integer
    , decimal
};

ENumericType get_numeric_type( const string& str )
{
    bool seen_dot = false;

    string::const_iterator it = str.begin();

    // Minus sign is valid at the beginning
    if( *it == '-' )
    {
        ++it;
        // but not if it's on its own
        if( it == str.end() )
        {
            return nonnumeric;
        }
    }

    for( ; it != str.end(); ++it )
    {
        char ch = *it;

        if( ch == '.' )
        {
            if( seen_dot )
            {
                return nonnumeric;
            }
            seen_dot = true;
        }
        else if( ch < '0' or ch > '9' )
        {
            return nonnumeric;
        }
    }

    if( seen_dot )
    {
        return decimal;
    }
    else
    {
        return integer;
    }
}

}

namespace ValueFactory
{

std::auto_ptr<Value> CreateValue( const StringTreeLeaf* leaf )
{
    const std::string& leaf_str = leaf->str();

    switch( get_numeric_type( leaf_str ) )
    {
        case integer:
        {
            // TODO: handle large integers
            istringstream ss( leaf_str );
            int intvalue;
            ss >> intvalue;
            return auto_ptr<Value>( new IntegerValue( intvalue ) );
        }
        case decimal:
        {
            // TODO: handle large decimals
            istringstream ss( leaf_str );
            double doublevalue;
            ss >> doublevalue;
            return auto_ptr<Value>( new DecimalValue( doublevalue ) );
        }
        default:
        {
            return auto_ptr<Value>( new SymbolValue( leaf_str ) );
        }
    }
}

};

