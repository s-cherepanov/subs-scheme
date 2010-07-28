
#include "assertmacros.h"
#include "subs/subsinterpreter.h"

#include "testconstants.h"

namespace
{

void five()
{
    TEST_ASSERT_EQUAL(
        SubsInterpreter().interpret( "5" ),
        "5" );
}

}

void TestConstants::run()
{
    five();
}

