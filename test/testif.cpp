
#include "assertmacros.h"
#include "lib/subsinterpreter.h"

#include "testif.h"

namespace
{

void true_and_false()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "#t" ), "#t" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "#f" ), "#f" );
}

void basic_if()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(if #t 10 20)" ), "10" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(if #f 10 20)" ), "20" );
}





}

void TestIf::Run() const
{
    true_and_false();
    basic_if();
    //TODO wrong_number_of_args_to_if_error_handled();
    //FAILS everything_is_true_except_hash_f();
}

