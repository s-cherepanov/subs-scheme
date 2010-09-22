/*
 *  Subs Scheme Interpreter
 *  Copyright (C) 2010 Andy Balaam
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**/

#include <string>

#include "assertmacros.h"
#include "lib/evaluationerror.h"
#include "lib/subsinterpreter.h"

#include "testarithmetic.h"

using namespace std;

namespace
{

void addition_of_two_numbers_yields_correct_answer()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(+ 1 2)" ), "3" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(+ 1.5 2.1)" ), "3.6" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(+ 1 2.1)" ), "3.1" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(+ 1.5 2)" ), "3.5" );
}


void addition_of_three_numbers_yields_correct_answer()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(+ 1 2 6)" ), "9" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(+ 1 2.1 6)" ), "9.1" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(+ 1.2 2 6)" ), "9.2" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(+ 1.2 2.1 6.3)" ),
        "9.6" );
}


void null_addition_yields_zero()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(+)" ), "0" );
}

void single_number_addition_yields_that_number()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(+ 4)" ), "4" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(+ 4.3)" ), "4.3" );
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
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(* 1.2 2)" ), "2.4" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(* 1 2.3)" ), "2.3" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(* 1.1 2.3)" ), "2.53" );
}


void multiplication_of_three_numbers_yields_correct_answer()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(* 1 2 6)" ), "12" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(* 1 2.1 6)" ), "12.6" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(* 2 2 6.2)" ), "24.8" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(* 1.1 2.2 6.3)" ),
        "15.246" );
}


void null_multiplication_yields_one()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(*)" ), "1" );
}

void single_number_multiplication_yields_that_number()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(* 4)" ), "4" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(* 4.3)" ), "4.3" );
}


void subtraction_of_two_numbers_yields_correct_answer()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(- 4 3)" ), "1" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(- 4 6)" ), "-2" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(- 4.1 3)" ), "1.1" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(- 4.1 6)" ), "-1.9" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(- 4 3.1)" ), "0.9" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(- 4 6.1)" ), "-2.1" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(- 4.1 3.1)" ), "1.0" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(- 4.2 6.1)" ), "-1.9" );
}

void subtraction_of_three_numbers_yields_correct_answer()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(- 4 1 1)" ), "2" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(- 4 1.1 1)" ), "1.9" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(- 4.1 1 1)" ), "2.1" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(- 4.1 1.1 1.1)" ),
        "1.9" );
}

void null_subtraction_is_an_error()
{
    TEST_ASSERT_THROWS_BEGIN
    {
        SubsInterpreter().Interpret( "(-)" );
    }
    TEST_ASSERT_THROWS_END( "Not enough" )
}

void single_number_subtraction_negates_that_number()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(- 4)" ), "-4" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(- -4)" ), "4" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(- 4.7)" ), "-4.7" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(- -4.7)" ), "4.7" );
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
    TEST_ASSERT_THROWS_BEGIN
    {
        SubsInterpreter().Interpret( "(/)" );
    }
    TEST_ASSERT_THROWS_END( "Not enough" )
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

void division_of_three_numbers_yields_correct_answer()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(/ 6 2 3)" ), "1" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(/ 1 2 2)" ), "0.25" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(/ 6.0 2 3)" ), "1.0" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(/ 6 1.5 1.6)" ), "2.5" );

    string ans = SubsInterpreter().Interpret( "(/ 23.2 2.1 3.1)" );
    TEST_ASSERT_EQUAL( ans.substr( 0, 4 ), "3.56" ); //TODO: arbitrary precision
}


void remainder_with_wrong_num_arguments_is_an_error()
{
    TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS( "remainder", 2 );
}


void remainder_with_nonint_arguments_is_an_error()
{
    SubsInterpreter interpreter;

    TEST_ASSERT_THROWS_BEGIN
    {
        interpreter.Interpret( "(remainder 5.1 2.1)" );
    }
    TEST_ASSERT_THROWS_END("Invalid argument")
}

void remainder_produces_modulus()
{
    SubsInterpreter interpreter;

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(remainder 6 2)" ), "0" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(remainder 5 2)" ), "1" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(remainder 8 3)" ), "2" );
}


}

#define SUITENAME "TestArithmetic"

void TestArithmetic::Run() const
{
    RUN_TEST(addition_of_two_numbers_yields_correct_answer);
    RUN_TEST(addition_of_three_numbers_yields_correct_answer);
    RUN_TEST(null_addition_yields_zero);
    RUN_TEST(single_number_addition_yields_that_number);
    RUN_TEST(nested_addition_works);

    RUN_TEST(multiplication_of_two_numbers_yields_correct_answer);
    RUN_TEST(multiplication_of_three_numbers_yields_correct_answer);
    RUN_TEST(null_multiplication_yields_one);
    RUN_TEST(single_number_multiplication_yields_that_number);

    RUN_TEST(subtraction_of_two_numbers_yields_correct_answer);
    RUN_TEST(subtraction_of_three_numbers_yields_correct_answer);
    RUN_TEST(null_subtraction_is_an_error);
    RUN_TEST(single_number_subtraction_negates_that_number);

    RUN_TEST(exact_division_of_two_integers_yields_integer_answer);
    RUN_TEST(inexact_division_of_two_integers_yields_decimal_answer);
    RUN_TEST(null_division_is_an_error);
    RUN_TEST(single_number_division_yields_reciprocal);
    RUN_TEST(division_involving_decimal_yields_decimal_answer);
    RUN_TEST(division_of_three_numbers_yields_correct_answer);

    RUN_TEST(remainder_with_wrong_num_arguments_is_an_error);
    RUN_TEST(remainder_with_nonint_arguments_is_an_error);
    RUN_TEST(remainder_produces_modulus);
}

