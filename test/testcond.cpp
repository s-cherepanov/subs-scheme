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

#include "testcond.h"

using namespace std;

namespace
{

void no_args_is_an_error()
{
    bool exception_caught = false;
    try
    {
        SubsInterpreter().Interpret( "(cond)" );
    }
    catch( EvaluationError& e )
    {
        exception_caught = true;
    }

    TEST_ASSERT_TRUE( exception_caught );
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
    bool exception_caught = false;
    try
    {
        SubsInterpreter().Interpret(
            "(cond ((= 0 1) 5)"
                  "(else 3)"
                  "((= 2 2) 4))" );
    }
    catch( EvaluationError& e )
    {
        exception_caught = true;
    }

    TEST_ASSERT_TRUE( exception_caught );
}


void non_combo_condition_is_an_error()
{
    bool exception_caught = false;
    try
    {
        SubsInterpreter().Interpret(
            "(cond 3"
                  "(else 3)"
                  "((= 2 2) 4))" );
    }
    catch( EvaluationError& e )
    {
        exception_caught = true;
    }

    TEST_ASSERT_TRUE( exception_caught );
}


void too_many_parts_in_condition_is_an_error()
{
    bool exception_caught = false;
    try
    {
        SubsInterpreter().Interpret(
            "(cond (#t 3 5)"
                  "(else 3)"
                  "((= 2 2) 4))" );
    }
    catch( EvaluationError& e )
    {
        exception_caught = true;
    }

    TEST_ASSERT_TRUE( exception_caught );
}


}

void TestCond::Run() const
{
    no_args_is_an_error();
    single_else_returns_value();
    single_true_condition_returns_value();
    single_false_condition_returns_nothing();
    multiple_conditions();
    else_after_multiple_conditions_including_one_true();
    else_after_multiple_conditions_all_false();
    pair_after_else_is_an_error();
    non_combo_condition_is_an_error();
    too_many_parts_in_condition_is_an_error();
}
