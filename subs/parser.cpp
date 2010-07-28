
#include <memory>

#include "integervalue.h"
#include "stringtree.h"
#include "stringtreeleaf.h"
#include "value.h"
#include "valuefactory.h"

#include "parser.h"

using namespace std;

std::auto_ptr<Value> Parser::parse( const StringTree& stringtree ) const
{
    const StringTreeLeaf* leaf = dynamic_cast<const StringTreeLeaf*>(
        &stringtree );
    if( leaf )
    {
        return ValueFactory::CreateValue( leaf );
    }
    else
    {
        // TODO: implement understanding non-trivial StringTrees
        return auto_ptr<Value>( NULL );
    }
}


