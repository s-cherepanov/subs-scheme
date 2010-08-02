
#include "assertmacros.h"
#include "lib/subsinterpreter.h"

#include "testcomparators.h"

namespace
{

void equals_works_for_integers()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(= 1 1)" ), "#t" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(= 1 2)" ), "#f" );

    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(= 1 1 1 1 1 1 1)" ),
        "#t" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(= 1 1 1 1 3 1 1)" ),
        "#f" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(= 3 1 1 1 1 1 1)" ),
        "#f" );
}


}

void TestComparators::Run() const
{
    equals_works_for_integers();
    //TODO too_few_operands_to_equals_is_an_error();
}

