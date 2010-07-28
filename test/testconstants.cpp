
#include "assertmacros.h"
#include "subs/subsinterpreter.h"

#include "testconstants.h"

namespace
{


void empty_string_yields_empty_string()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "" ), "" );
}


void bare_number_yields_itself()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "5" ), "5" );
}

}

void TestConstants::Run() const
{
    empty_string_yields_empty_string();
    bare_number_yields_itself();
}

