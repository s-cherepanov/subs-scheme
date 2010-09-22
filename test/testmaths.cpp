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

void log_no_args_is_an_error()
{
    bool exception_caught = false;
    try
    {
        SubsInterpreter().Interpret( "(log)" );
    }
    catch( EvaluationError& )
    {
        exception_caught = true;
    }
    TEST_ASSERT_TRUE( exception_caught );
}


void log_two_args_is_an_error()
{
    bool exception_caught = false;
    try
    {
        SubsInterpreter().Interpret( "(log 1 2)" );
    }
    catch( EvaluationError& )
    {
        exception_caught = true;
    }
    TEST_ASSERT_TRUE( exception_caught );
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
    SubsInterpreter interpreter;

    TEST_ASSERT_THROWS_BEGIN
    {
        interpreter.Interpret( "(exp)" );
    }
    TEST_ASSERT_THROWS_END("Not enough")

    TEST_ASSERT_THROWS_BEGIN
    {
        interpreter.Interpret( "(exp 1 2)" );
    }
    TEST_ASSERT_THROWS_END("Too many")
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
    SubsInterpreter interpreter;

    TEST_ASSERT_THROWS_BEGIN
    {
        interpreter.Interpret( "(sin)" );
    }
    TEST_ASSERT_THROWS_END("Not enough")

    TEST_ASSERT_THROWS_BEGIN
    {
        interpreter.Interpret( "(sin 1 2)" );
    }
    TEST_ASSERT_THROWS_END("Too many")
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
    SubsInterpreter interpreter;

    TEST_ASSERT_THROWS_BEGIN
    {
        interpreter.Interpret( "(cos)" );
    }
    TEST_ASSERT_THROWS_END("Not enough")

    TEST_ASSERT_THROWS_BEGIN
    {
        interpreter.Interpret( "(cos 1 2)" );
    }
    TEST_ASSERT_THROWS_END("Too many")
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
    SubsInterpreter interpreter;

    TEST_ASSERT_THROWS_BEGIN
    {
        interpreter.Interpret( "(tan)" );
    }
    TEST_ASSERT_THROWS_END("Not enough")

    TEST_ASSERT_THROWS_BEGIN
    {
        interpreter.Interpret( "(tan 1 2)" );
    }
    TEST_ASSERT_THROWS_END("Too many")
}


void tan_returns_tangent()
{
    SubsInterpreter interpreter;
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(tan 1)" ), "1.55741" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(tan 2)" ), "-2.18504" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(tan 10)" ), "0.648361" );
}





}

#define SUITENAME "TestMaths"

void TestMaths::Run() const
{
    RUN_TEST(log_no_args_is_an_error);
    RUN_TEST(log_two_args_is_an_error);
    RUN_TEST(log_calculates_ln_base_e);
    RUN_TEST(exp_wrong_number_of_args_is_an_error);
    RUN_TEST(exp_returns_e_to_the_x);
    RUN_TEST(sin_wrong_number_of_args_is_an_error);
    RUN_TEST(sin_returns_sine);
    RUN_TEST(cos_wrong_number_of_args_is_an_error);
    RUN_TEST(cos_returns_cosine);
    RUN_TEST(tan_wrong_number_of_args_is_an_error);
    RUN_TEST(tan_returns_tangent);
}

