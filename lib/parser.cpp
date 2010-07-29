
#include <cassert>
#include <memory>

#include "combinationvalue.h"
#include "integervalue.h"
#include "stringtree.h"
#include "stringtreebranch.h"
#include "stringtreeleaf.h"
#include "value.h"
#include "valuefactory.h"

#include "parser.h"

using namespace std;

std::auto_ptr<Value> Parser::Parse( const StringTree* stringtree ) const
{
   const StringTreeBranch* branch = dynamic_cast<const StringTreeBranch*>(
        stringtree );
    if( branch )
    {
        // TODO: can we do this easily without recursion?
        auto_ptr<CombinationValue> ret( new CombinationValue );
        for( StringTreeBranch::const_iterator it = branch->begin();
            it != branch->end(); ++it )
        {
            ret->push_back( Parse( *it ).release() );
        }
        return auto_ptr<Value>( ret.release() );
    }
    else
    {
        const StringTreeLeaf* leaf = dynamic_cast<const StringTreeLeaf*>(
            stringtree );
        if( leaf )
        {// If this is not a branch, it must be a leaf
            return ValueFactory::CreateValue( leaf );
        }
        else
        {
            return auto_ptr<Value>( NULL );
        }
    }
}


