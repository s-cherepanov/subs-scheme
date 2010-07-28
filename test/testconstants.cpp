
#include "assertmacros.h"
#include "subs/subsinterpreter.h"

#include "testconstants.h"

namespace
{


void empty_string_yields_empty_string()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().interpret( "" ), "" );
}


void bare_number_yields_itself()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().interpret( "5" ), "5" );
}

}

void TestConstants::run() const
{
    empty_string_yields_empty_string();
    // FAILS bare_number_yields_itself();
}

