
#include "assertmacros.h"
#include "lib/subsinterpreter.h"

#include "testarithmetic.h"

namespace
{

void addition_of_two_numbers_yields_correct_answer()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(+ 1 2)" ), "3" );
}


void addition_of_three_numbers_yields_correct_answer()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(+ 1 2 6)" ), "9" );
}


void null_addition_yields_zero()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(+)" ), "0" );
}

void single_number_addition_yields_that_number()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(+ 4)" ), "4" );
}


void nested_addition_works()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret(
        "(+ (+ 1 2 (+ 2 3 4)) 4)" ),
        "16" );
}





}

void TestArithmetic::Run() const
{
    addition_of_two_numbers_yields_correct_answer();
    addition_of_three_numbers_yields_correct_answer();
    null_addition_yields_zero();
    single_number_addition_yields_that_number();
    nested_addition_works();
}

