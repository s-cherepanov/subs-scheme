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

#include "assertmacros.h"
#include "lib/evaluationerror.h"
#include "lib/subsinterpreter.h"

#include "testmaths.h"

using namespace std;

namespace
{

void log_wrong_number_of_args_is_an_error()
{
    TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS( "log", 1 );
}


void log_calculates_ln_base_e()
{
    SubsInterpreter interpreter;
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(log 1)" ), "0.0" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(log 2)" ), "0.693147" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(log 10)" ), "2.30259" );
}

void exp_wrong_number_of_args_is_an_error()
{
    TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS( "exp", 1 );
}


void exp_returns_e_to_the_x()
{
    SubsInterpreter interpreter;
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(exp 1)" ), "2.71828" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(exp 2)" ), "7.38906" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(exp 10)" ), "22026.5" );
}



void sin_wrong_number_of_args_is_an_error()
{
    TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS( "sin", 1 );
}


void sin_returns_sine()
{
    SubsInterpreter interpreter;
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(sin 1)" ), "0.841471" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(sin 2)" ), "0.909297" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(sin 10)" ), "-0.544021" );
}



void cos_wrong_number_of_args_is_an_error()
{
    TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS( "cos", 1 );
}


void cos_returns_cosine()
{
    SubsInterpreter interpreter;
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(cos 1)" ), "0.540302" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(cos 2)" ), "-0.416147" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(cos 10)" ), "-0.839072" );
}



void tan_wrong_number_of_args_is_an_error()
{
    TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS( "tan", 1 );
}


void tan_returns_tangent()
{
    SubsInterpreter interpreter;
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(tan 1)" ), "1.55741" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(tan 2)" ), "-2.18504" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(tan 10)" ), "0.648361" );
}



void zero_wrong_number_of_args_is_an_error()
{
    TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS( "zero?", 1 );
}


void zero_returns_equal_zero()
{
    SubsInterpreter interpreter;
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(zero? 1)" ), "#f" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(zero? -1.5)" ), "#f" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(zero? 0)" ), "#t" );
}


void positive_wrong_number_of_args_is_an_error()
{
    TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS( "positive?", 1 );
}


void positive_returns_greater_than_zero()
{
    SubsInterpreter interpreter;
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(positive? 1)" ), "#t" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(positive? -1.5)" ), "#f" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(positive? 0)" ), "#f" );
}

void negative_wrong_number_of_args_is_an_error()
{
    TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS( "negative?", 1 );
}


void negative_returns_less_than_zero()
{
    SubsInterpreter interpreter;
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(negative? 1)" ), "#f" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(negative? -1.5)" ), "#t" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(negative? 0)" ), "#f" );
}



void min_with_no_args_is_an_error()
{
    TEST_ASSERT_THROWS_BEGIN
    {
        SubsInterpreter().Interpret( "(min)" );
    }
    TEST_ASSERT_THROWS_END( "Not enough" )
}



void min_returns_the_minimum()
{
    SubsInterpreter interpreter;
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(min 1)" ), "1" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(min 5 12 3 6)" ), "3" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(min 2.7 3.3)" ), "2.7" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(min 3 -1.5 2.7)" ), "-1.5" );
}



void max_with_no_args_is_an_error()
{
    TEST_ASSERT_THROWS_BEGIN
    {
        SubsInterpreter().Interpret( "(max)" );
    }
    TEST_ASSERT_THROWS_END( "Not enough" )
}


void max_returns_the_maximum()
{
    SubsInterpreter interpreter;
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(max 1)" ), "1" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(max 5 12 3 6)" ), "12" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(max 2.7 3.3)" ), "3.3" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(max 3 -1.5 2.7)" ), "3.0" );
}


void odd_wrong_number_of_args_is_an_error()
{
    TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS( "odd?", 1 );
}


void odd_returns_not_divisible_by_2()
{
    SubsInterpreter interpreter;
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(odd? 1)" ), "#t" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(odd? 12)" ), "#f" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(odd? 13)" ), "#t" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(odd? 0)" ), "#f" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(odd? -3)" ), "#t" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(odd? -4)" ), "#f" );
}



void even_wrong_number_of_args_is_an_error()
{
    TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS( "even?", 1 );
}


void even_returns_divisible_by_2()
{
    SubsInterpreter interpreter;
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(even? 1)" ), "#f" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(even? 12)" ), "#t" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(even? 13)" ), "#f" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(even? 0)" ), "#t" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(even? -3)" ), "#f" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(even? -4)" ), "#t" );
}



}

#define SUITENAME "TestMaths"

void TestMaths::Run() const
{
    RUN_TEST(log_wrong_number_of_args_is_an_error);
    RUN_TEST(log_calculates_ln_base_e);
    RUN_TEST(exp_wrong_number_of_args_is_an_error);
    RUN_TEST(exp_returns_e_to_the_x);
    RUN_TEST(sin_wrong_number_of_args_is_an_error);
    RUN_TEST(sin_returns_sine);
    RUN_TEST(cos_wrong_number_of_args_is_an_error);
    RUN_TEST(cos_returns_cosine);
    RUN_TEST(tan_wrong_number_of_args_is_an_error);
    RUN_TEST(tan_returns_tangent);
    RUN_TEST(zero_wrong_number_of_args_is_an_error);
    RUN_TEST(zero_returns_equal_zero);
    RUN_TEST(positive_wrong_number_of_args_is_an_error);
    RUN_TEST(positive_returns_greater_than_zero);
    RUN_TEST(negative_wrong_number_of_args_is_an_error);
    RUN_TEST(negative_returns_less_than_zero);
    RUN_TEST(min_with_no_args_is_an_error);
    RUN_TEST(min_returns_the_minimum);
    RUN_TEST(max_with_no_args_is_an_error);
    RUN_TEST(max_returns_the_maximum);
    RUN_TEST(odd_wrong_number_of_args_is_an_error);
    RUN_TEST(odd_returns_not_divisible_by_2);
    RUN_TEST(even_wrong_number_of_args_is_an_error);
    RUN_TEST(even_returns_divisible_by_2);
}

