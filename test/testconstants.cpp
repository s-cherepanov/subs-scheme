
#include "assertmacros.h"
#include "lib/evaluationerror.h"
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


void negative_number_yields_itself()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "-5" ), "-5" );
}


void decimal_number_yields_itself()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "1.45" ), "1.45" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( ".45" ), "0.45" );
}


void end_in_dot_is_failure()
{
    bool exception_caught = false;
    try
    {
        SubsInterpreter().Interpret( "1." );
    }
    catch( EvaluationError& )
    {
        exception_caught = true;
    }

    TEST_ASSERT_TRUE( exception_caught );
}



void two_dots_is_failure()
{
    bool exception_caught = false;
    try
    {
        SubsInterpreter().Interpret( "1.4.5" );
    }
    catch( EvaluationError& )
    {
        exception_caught = true;
    }

    TEST_ASSERT_TRUE( exception_caught );
}




//void combo_yields_itself()
//{
//    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(4 5)" ), "(4 5)" );
//}
//
//
//
//void combo_of_combos_yields_itself()
//{
//    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(4 (5 4 1))" ),
//        "(4 (5 4 1))" );
//}



}

void TestConstants::Run() const
{
    empty_string_yields_empty_string();
    bare_number_yields_itself();
    negative_number_yields_itself();
    decimal_number_yields_itself();
    end_in_dot_is_failure();
    two_dots_is_failure();
    //TODO unparseable_integer_is_an_error();
    //INCORRECT could quote combo_yields_itself();
    //INCORRECT could quote combo_of_combos_yields_itself();
}

