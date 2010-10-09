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

#include "testlists.h"

using namespace std;

namespace
{


void list_creates_nested_pairs()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define x (list 1 2 3))" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(car x)" ), "1" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(cdr x)" ), "(2 3)" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(car (cdr x))" ), "2" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(car (cdr (cdr x)))" ), "3" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(cdr (cdr (cdr x)))" ), "()" );
}



void null_with_wrong_number_of_args_is_an_error()
{
    TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS( "null?", 1 );
}



void null_returns_true_if_nil()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define x nil)" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(null? 1)" ), "#f" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(null? (list 1 2))" ), "#f" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(null? (list 1))" ), "#f" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(null? (list))" ), "#t" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(null? nil)" ), "#t" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(null? x)" ), "#t" );
}




void map_applies_function_to_each_item()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define (plus1 x) (+ x 1))" );

    TEST_ASSERT_EQUAL(interpreter.Interpret(
        "(map plus1 (list 4 6 9))" ), "(5 7 10)" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(map plus1 (list))" ), "()" );
}



void map_requires_2_args()
{
    TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS( "map", 2 );
}


void map_requires_a_function_first()
{
    TEST_ASSERT_THROWS_BEGIN
    {
        SubsInterpreter().Interpret( "(map 1 (list 2 3))" );
    }
    TEST_ASSERT_THROWS_END( "Attempted to run '1', which is not a procedure." )
}




void map_requires_a_unary_function_first()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define (myfn x y) 1)" );

    TEST_ASSERT_THROWS_BEGIN
    {
        interpreter.Interpret( "(map myfn (list 2 3))" );
    }
    TEST_ASSERT_THROWS_END( "Not enough operands to myfn" )
}



void map_requires_a_pair_second()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define (plus1 x) (+ x 1))" );

    TEST_ASSERT_THROWS_BEGIN
    {
        interpreter.Interpret( "(map plus1 1)" );
    }
    TEST_ASSERT_THROWS_END( "'1' is not a pair" )
}



void map_requires_a_list_second()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define (plus1 x) (+ x 1))" );

    TEST_ASSERT_THROWS_BEGIN
    {
        interpreter.Interpret( "(map plus1 (cons 1 2))" );
    }
    TEST_ASSERT_THROWS_END( "'2' is not a pair" )
}



void length_requires_one_arg()
{
    TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS( "length", 1 );
}


void length_returns_num_items()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define x (list 1 2 3 4))" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(length x)" ), "4" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(length (list 1 2 3))" ), "3" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(length (list 1 2))" ), "2" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(length (list 1))" ), "1" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(length (list))" ), "0" );
}



void append_requires_2_args()
{
    TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS( "append", 2 );
}


void append_concatenates_2_lists()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define x (list 1 2))" );
    interpreter.Interpret( "(define y (list 3 4))" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(append x y)" ), "(1 2 3 4)" );
}




void list_ref_requires_2_args()
{
    TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS( "list-ref", 2 );
}


void list_ref_finds_an_item_in_the_list()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define x (list 1 2 3 4))" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(list-ref x 0)" ), "1" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(list-ref x 2)" ), "3" );
}




}

#define SUITENAME "TestLists"

void TestLists::Run() const
{
    RUN_TEST(list_creates_nested_pairs);
    RUN_TEST(null_with_wrong_number_of_args_is_an_error);
    RUN_TEST(null_returns_true_if_nil);
    RUN_TEST(map_applies_function_to_each_item);
    RUN_TEST(map_requires_2_args);
    RUN_TEST(map_requires_a_function_first);
    RUN_TEST(map_requires_a_unary_function_first);
    RUN_TEST(map_requires_a_pair_second);
    RUN_TEST(map_requires_a_list_second);
    RUN_TEST(length_requires_one_arg);
    RUN_TEST(length_returns_num_items);
    RUN_TEST(append_requires_2_args);
    RUN_TEST(append_concatenates_2_lists);
    RUN_TEST(list_ref_requires_2_args);
    RUN_TEST(list_ref_finds_an_item_in_the_list);
}

