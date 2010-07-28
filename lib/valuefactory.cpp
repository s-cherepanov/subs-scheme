
#include <memory>
#include <sstream>

#include "integervalue.h"
#include "stringtreeleaf.h"
#include "value.h"

#include "valuefactory.h"

using namespace std;

namespace ValueFactory
{

std::auto_ptr<Value> CreateValue( const StringTreeLeaf* leaf )
{
    // TODO: handle types other than int
    {
        // TODO: handle different sorts of int e.g larger, unsigned?
        istringstream ss( leaf->str() );
        int intvalue;
        ss >> intvalue;
        return auto_ptr<Value>( new IntegerValue( intvalue ) );
    }
}

};

