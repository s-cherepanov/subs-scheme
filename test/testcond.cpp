/*
 *  Subs Scheme Interpreter
 *  Copyright (C) 2010, 2011 Andy Balaam
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

#include "lib/evaluationerror.h"
#include "lib/subsinterpreter.h"
#include "test/assertmacros.h"
#include "test/testcond.h"

using namespace std;

namespace
{

void no_args_is_an_error()
{
    TEST_ASSERT_THROWS_BEGIN
    {
        SubsInterpreter().Interpret( "(cond)" );
    }
    TEST_ASSERT_THROWS_END( "Not enough" )
}



void single_else_returns_value()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret(
        "(cond (else 4))" ),
        "4" );
}


void single_true_condition_returns_value()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret(
        "(cond (#t 4))" ),
        "4" );
}


void single_false_condition_returns_nothing()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret(
        "(cond (#f 4))" ),
        "" );
}


void multiple_conditions()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret(
        "(cond ((= 3 2) 4)"
              "((= 1 0) 3)"
              "((= 5 5) 2)"
              "((= 2 6) 1))" ),
        "2" );
}


void else_after_multiple_conditions_including_one_true()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret(
        "(cond ((= 3 2) 4)"
              "((= 1 0) 3)"
              "((= 5 5) 2)"
              "((= 2 6) 1)"
              "(else    0))" ),
        "2" );
}


void else_after_multiple_conditions_all_false()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret(
        "(cond ((= 3 2) 4)"
              "((= 1 0) 3)"
              "((= 5 7) 2)"
              "((= 2 6) 1)"
              "(else    0))" ),
        "0" );
}


void pair_after_else_is_an_error()
{
    TEST_ASSERT_THROWS_BEGIN
    {
        SubsInterpreter().Interpret(
            "(cond ((= 0 1) 5)"
                  "(else 3)"
                  "((= 2 2) 4))" );
    }
    TEST_ASSERT_THROWS_END( "The else pair must be the last pair" )
}


void non_combo_condition_is_an_error()
{
    TEST_ASSERT_THROWS_BEGIN
    {
        SubsInterpreter().Interpret(
            "(cond 3"
                  "((= 2 2) 4))"
                  "(else 3)" );
    }
    TEST_ASSERT_THROWS_END( "The operands to 'cond' must be pairs" )
}


void too_many_parts_in_condition_is_an_error()
{
    TEST_ASSERT_THROWS_BEGIN
    {
        SubsInterpreter().Interpret(
            "(cond (#t 3 5)"
                  "(else 3)"
                  "((= 2 2) 4))" );
    }
    TEST_ASSERT_THROWS_END( "The operands to 'cond' must be pairs" )
}


}

#define SUITENAME "TestCond"

void TestCond::Run() const
{
    RUN_TEST(no_args_is_an_error);
    RUN_TEST(single_else_returns_value);
    RUN_TEST(single_true_condition_returns_value);
    RUN_TEST(single_false_condition_returns_nothing);
    RUN_TEST(multiple_conditions);
    RUN_TEST(else_after_multiple_conditions_including_one_true);
    RUN_TEST(else_after_multiple_conditions_all_false);
    RUN_TEST(pair_after_else_is_an_error);
    RUN_TEST(non_combo_condition_is_an_error);
    RUN_TEST(too_many_parts_in_condition_is_an_error);
}

