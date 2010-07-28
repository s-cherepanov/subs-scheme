
#include <memory>

#include "assertmacros.h"
#include "subs/integervalue.h"
#include "subs/parser.h"
#include "subs/stringtreeleaf.h"
#include "subs/value.h"

#include "testparseconstants.h"

using namespace std;

namespace
{

void bare_number_becomes_single_value()
{
    auto_ptr<Value> v = Parser().parse( StringTreeLeaf( "5" ) );

    IntegerValue* parsed_5_leaf = dynamic_cast<IntegerValue*>( v.get() );

    TEST_ASSERT_NOT_NULL( parsed_5_leaf );

    TEST_ASSERT_EQUAL( parsed_5_leaf->GetIntValue(), 5 );
}

}

void TestParseConstants::run() const
{
    bare_number_becomes_single_value();
}

