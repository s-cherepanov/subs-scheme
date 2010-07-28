
#include <memory>

#include "assertmacros.h"
#include "lib/integervalue.h"
#include "lib/parser.h"
#include "lib/stringtreeleaf.h"
#include "lib/value.h"

#include "testparseconstants.h"

using namespace std;

namespace
{

void bare_number_becomes_single_value()
{
    StringTreeLeaf leaf_5( "5" );
    auto_ptr<Value> v = Parser().Parse( &leaf_5 );

    IntegerValue* parsed_5_leaf = dynamic_cast<IntegerValue*>( v.get() );

    TEST_ASSERT_NOT_NULL( parsed_5_leaf );

    TEST_ASSERT_EQUAL( parsed_5_leaf->GetIntValue(), 5 );
}

}

void TestParseConstants::Run() const
{
    bare_number_becomes_single_value();
}

