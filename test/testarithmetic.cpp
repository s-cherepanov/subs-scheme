
#include "assertmacros.h"
#include "lib/evaluationerror.h"
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



void multiplication_of_two_numbers_yields_correct_answer()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(* 1 2)" ), "2" );
}


void multiplication_of_three_numbers_yields_correct_answer()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(* 1 2 6)" ), "12" );
}


void null_multiplication_yields_one()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(*)" ), "1" );
}

void single_number_multiplication_yields_that_number()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(* 4)" ), "4" );
}


void subtraction_of_two_numbers_yields_correct_answer()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(- 4 3)" ), "1" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(- 4 6)" ), "-2" );
}

void subtraction_of_three_numbers_yields_correct_answer()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(- 4 1 1)" ), "2" );
}

void null_subtraction_is_an_error()
{
    bool exception_caught = false;
    try
    {
        SubsInterpreter().Interpret( "(-)" );
    }
    catch( EvaluationError& e )
    {
        exception_caught = true;
    }
    TEST_ASSERT_TRUE( exception_caught );
}

void single_number_subtraction_negates_that_number()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(- 4)" ), "-4" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(- -4)" ), "4" );
}


void exact_division_of_two_integers_yields_integer_answer()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(/ 6 2)" ), "3" );
}


void inexact_division_of_two_integers_yields_decimal_answer()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(/ 2 8)" ), "0.25" );
}

void null_division_is_an_error()
{
    bool exception_caught = false;
    try
    {
        SubsInterpreter().Interpret( "(/)" );
    }
    catch( EvaluationError& e )
    {
        exception_caught = true;
    }
    TEST_ASSERT_TRUE( exception_caught );
}


void single_number_division_yields_reciprocal()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(/ 2)" ), "0.5" );
}


void division_involving_decimal_yields_decimal_answer()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(/ 2.0)" ), "0.5" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(/ 0.5)" ), "2.0" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(/ 1.5 2)" ), "0.75" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(/ 6 1.5)" ), "4.0" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(/ 1.5 3)" ), "0.5" );
}


}

void TestArithmetic::Run() const
{
    addition_of_two_numbers_yields_correct_answer();
    addition_of_three_numbers_yields_correct_answer();
    null_addition_yields_zero();
    single_number_addition_yields_that_number();
    nested_addition_works();

    multiplication_of_two_numbers_yields_correct_answer();
    multiplication_of_three_numbers_yields_correct_answer();
    null_multiplication_yields_one();
    single_number_multiplication_yields_that_number();

    subtraction_of_two_numbers_yields_correct_answer();
    subtraction_of_three_numbers_yields_correct_answer();
    null_subtraction_is_an_error();
    single_number_subtraction_negates_that_number();

    exact_division_of_two_integers_yields_integer_answer();
    inexact_division_of_two_integers_yields_decimal_answer();
    null_division_is_an_error();
    single_number_division_yields_reciprocal();
    division_involving_decimal_yields_decimal_answer();
    //TODO: division_of_three_numbers_yields_correct_answer();
}

