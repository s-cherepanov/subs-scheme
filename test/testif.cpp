
#include <string>

#include "assertmacros.h"
#include "lib/evaluationerror.h"
#include "lib/subsinterpreter.h"

#include "testif.h"

using namespace std;

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

void too_few_args_to_if_is_an_error()
{
    bool exception_caught = false;
    try
    {
        SubsInterpreter().Interpret( "(if #t 1)" );
    }
    catch( EvaluationError& e )
    {
        TEST_ASSERT_NOT_EQUAL( e.ToString().find( "Not enough" ),
            string::npos );
        exception_caught = true;
    }

    TEST_ASSERT_TRUE( exception_caught );
}

void too_many_args_to_if_is_an_error()
{
    bool exception_caught = false;
    try
    {
        SubsInterpreter().Interpret( "(if #t 1 2 3)" );
    }
    catch( EvaluationError& e )
    {
        TEST_ASSERT_NOT_EQUAL( e.ToString().find( "Too many" ), string::npos );
        exception_caught = true;
    }

    TEST_ASSERT_TRUE( exception_caught );
}



}

void TestIf::Run() const
{
    true_and_false();
    basic_if();
    too_few_args_to_if_is_an_error();
    too_many_args_to_if_is_an_error();
    //FAILS everything_is_true_except_hash_f();
}

