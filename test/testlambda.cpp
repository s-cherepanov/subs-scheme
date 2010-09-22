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

#include "testlambda.h"

using namespace std;

namespace
{


void define_lambda_noargs()
{
    SubsInterpreter interpreter;

    TEST_ASSERT_EQUAL( interpreter.Interpret(
        "(define foo (lambda () 3))" ),
        "foo" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "foo" ),
        "<<COMPOUND-PROCEDURE>>" );
}




void define_lambda_args()
{
    SubsInterpreter interpreter;

    TEST_ASSERT_EQUAL( interpreter.Interpret(
        "(define foo (lambda (x) x))" ),
        "foo" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "foo" ),
        "<<COMPOUND-PROCEDURE>>" );
}




void noargs_function_works()
{
    SubsInterpreter interpreter;

    TEST_ASSERT_EQUAL( interpreter.Interpret(
        "(define foo (lambda () (+ 1 2)))" ),
        "foo" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(foo)" ),
        "3" );
}




void noargs_function_works_list_body()
{
    SubsInterpreter interpreter;

    // The return value is 7 because the 5 and 6 are eval'd then ignored.
    TEST_ASSERT_EQUAL( interpreter.Interpret(
        "(define foo (lambda () 5 6 7))" ),
        "foo" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(foo)" ),
        "7" );
}





void onearg_function_works()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define double (lambda (x) (+ x x)))" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(double 3)" ), "6" );
}


void threearg_function_works()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define foo (lambda (x y z) (+ x x y z)))" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(foo 1 2 3)" ), "7" );
}




void error_when_supply_too_few_args()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define tri (lambda (x y z) 3))" );

    TEST_ASSERT_THROWS_BEGIN
    {
        interpreter.Interpret( "(tri 3 2)" );
    }
    TEST_ASSERT_THROWS_END2( "Not enough", "Expected 3 but got 2" )
}


void error_when_supply_too_many_args()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define tri (lambda (x y z) 3))" );

    TEST_ASSERT_THROWS_BEGIN
    {
        interpreter.Interpret( "(tri 3 2 1 0)" );
    }
    TEST_ASSERT_THROWS_END2( "Too many", "Expected 3 but got 4" )
}




void arguments_dont_leak_out()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define foo 3)" );
    interpreter.Interpret( "(define bar (lambda (foo) (+ foo 1)))" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(bar 1)" ), "2" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "foo" ), "3" );
}



}

#define SUITENAME "TestLambda"

void TestLambda::Run() const
{
    RUN_TEST(define_lambda_noargs);
    RUN_TEST(define_lambda_args);
    RUN_TEST(noargs_function_works);
    RUN_TEST(noargs_function_works_list_body);
    RUN_TEST(onearg_function_works);
    RUN_TEST(threearg_function_works);
    RUN_TEST(error_when_supply_too_few_args);
    RUN_TEST(error_when_supply_too_many_args);
    RUN_TEST(arguments_dont_leak_out);
}

