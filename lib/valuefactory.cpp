
#include <memory>
#include <sstream>
#include <string>

#include "integervalue.h"
#include "stringtreeleaf.h"
#include "symbolvalue.h"
#include "value.h"

#include "valuefactory.h"

using namespace std;

namespace
{

bool is_all_numbers( const string& str )
{
    for( string::const_iterator it = str.begin(); it != str.end(); ++it )
    {
        char ch = *it;
        if( ch < '0' or ch > '9' )
        {
            return false;
        }
    }
    return true;
}

}

namespace ValueFactory
{

std::auto_ptr<Value> CreateValue( const StringTreeLeaf* leaf )
{
    const std::string& leaf_str = leaf->str();
    if( is_all_numbers( leaf_str ) )
    {
        // TODO: handle different sorts of int e.g larger, unsigned?
        istringstream ss( leaf_str );
        int intvalue;
        ss >> intvalue;
        return auto_ptr<Value>( new IntegerValue( intvalue ) );
    }
    else
    {
        return auto_ptr<Value>( new SymbolValue( leaf_str ) );
    }
}

};

