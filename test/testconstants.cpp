
#include "assertmacros.h"
#include "lib/subsinterpreter.h"

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


void combo_yields_itself()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(4 5)" ), "(4 5)" );
}



void combo_of_combos_yields_itself()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(4 (5 4 1))" ),
        "(4 (5 4 1))" );
}



}

void TestConstants::Run() const
{
    empty_string_yields_empty_string();
    bare_number_yields_itself();
    combo_yields_itself();
    combo_of_combos_yields_itself();
}

