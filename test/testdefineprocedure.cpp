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

#include "testdefineprocedure.h"

using namespace std;

namespace
{


void define_proc_noargs()
{
    SubsInterpreter interpreter;

    TEST_ASSERT_EQUAL( interpreter.Interpret(
        "(define (foo) 3)" ),
        "foo" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "foo" ),
        "<<COMPOUND-PROCEDURE foo>>" );
}




void define_proc_args()
{
    SubsInterpreter interpreter;

    TEST_ASSERT_EQUAL( interpreter.Interpret(
        "(define (foo x y) x)" ),
        "foo" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "foo" ),
        "<<COMPOUND-PROCEDURE foo>>" );
}




void noargs_proc_works()
{
    SubsInterpreter interpreter;

    TEST_ASSERT_EQUAL( interpreter.Interpret(
        "(define (foo) (+ 1 2))" ),
        "foo" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(foo)" ),
        "3" );
}




void noargs_proc_works_list_body()
{
    SubsInterpreter interpreter;

    // The return value is 7 because the 5 and 6 are eval'd then ignored.
    TEST_ASSERT_EQUAL( interpreter.Interpret(
        "(define (foo) 5 6 7)" ),
        "foo" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(foo)" ),
        "7" );
}





void onearg_proc_works()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define (double x) (+ x x))" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(double 3)" ), "6" );
}


void threearg_proc_works()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define (foo x y z) (+ x x y z))" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(foo 1 2 3)" ), "7" );
}




void error_when_supply_too_few_args()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define (tri x y z) 3)" );

    TEST_ASSERT_THROWS_BEGIN
    {
        interpreter.Interpret( "(tri 3 2)" );
    }
    TEST_ASSERT_THROWS_END2( "Not enough", "were 2 but there should be 3" )
}


void error_when_supply_too_many_args()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define (tri x y z) 3)" );

    TEST_ASSERT_THROWS_BEGIN
    {
        interpreter.Interpret( "(tri 3 2 1 0)" );
    }
    TEST_ASSERT_THROWS_END2( "Too many", "were 4 but there should be 3" )
}




void proc_arguments_dont_leak_out()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define foo 3)" );
    interpreter.Interpret( "(define (bar foo) (+ foo 1))" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(bar 1)" ), "2" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "foo" ), "3" );
}



void define_doesnt_leak_out()
{
    SubsInterpreter interpreter;

    interpreter.Interpret(
        "(define (foo)"
        "        (define x 3)"
        "        x)"
        );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(foo)" ), "3" );

    TEST_ASSERT_THROWS_BEGIN
    {
        interpreter.Interpret( "x" );
    }
    TEST_ASSERT_THROWS_END( "Undefined symbol 'x'" )
}


void recursive_procedure()
{
    SubsInterpreter interpreter;

    // SICP 1.2.1
    interpreter.Interpret(
        "(define (factorial n)"
        "        (if (= n 1)"
        "            1"
        "            (* n (factorial (- n 1)))))"
        );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(factorial 1)" ), "1" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(factorial 2)" ), "2" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(factorial 3)" ), "6" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(factorial 5)" ), "120" );
}



void double_recursive_procedure()
{
    SubsInterpreter interpreter;

    interpreter.Interpret(
        "(define (bad-fib n)"
        "        (if (= n 1)"
        "            1"
        "            (if (= n 2)"
        "                1"
        "                (+ (bad-fib (- n 1)) (bad-fib (- n 2))))))"
        );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(bad-fib 1)" ), "1" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(bad-fib 2)" ), "1" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(bad-fib 3)" ), "2" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(bad-fib 4)" ), "3" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(bad-fib 5)" ), "5" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(bad-fib 6)" ), "8" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(bad-fib 7)" ), "13" );
}


void define_proc_inside_proc()
{
    SubsInterpreter interpreter;

    interpreter.Interpret(
        "(define (add-two-three n)"
        "        (define (add-two n)"
        "                (+ n 2))"
        "        (+ (add-two n) 3 ))"
        );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(add-two-three 2)" ), "7" );
}



void define_proc_inside_proc_overrides_previous()
{
    SubsInterpreter interpreter;

    interpreter.Interpret(
        "(define (add-two x y)"
        "        1)" );
    interpreter.Interpret(
        "(define (add-two-three n)"
        "        (define (add-two n)"
        "                (+ n 2))"
        "        (+ (add-two n) 3 ))"
        );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(add-two-three 2)" ), "7" );
}


void good_enough_bug()
{
    SubsInterpreter interpreter;

    interpreter.Interpret(
        "(define (square x)"
        "        (* x x))" );

    interpreter.Interpret(
        "(define (average x y)"
        "        (/ (+ x y) 2))" );

    interpreter.Interpret(
        "(define (abs x)"
        "        (cond ((< x 0) (- x))"
        "              (else x)))" );

    interpreter.Interpret(
        "(define (sqrt x)"
        "        (define (good-enough? guess)"
        "                (< (abs (- (square guess) x)) 0.001))"
        "        (define (improve guess)"
        "                (average guess (/ x guess)))"
        "        (define (sqrt-iter guess)"
        "                (if (good-enough? guess)"
        "                    guess"
        "                    (sqrt-iter (improve guess))))"
        "        (sqrt-iter 1.0))" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(sqrt 16)" ), "4.0" );
}


}

#define SUITENAME "TestDefineProcedure"

void TestDefineProcedure::Run() const
{
    RUN_TEST(define_proc_noargs);
    RUN_TEST(define_proc_args);
    RUN_TEST(noargs_proc_works);
    RUN_TEST(noargs_proc_works_list_body);
    RUN_TEST(onearg_proc_works);
    RUN_TEST(threearg_proc_works);
    RUN_TEST(error_when_supply_too_few_args);
    RUN_TEST(error_when_supply_too_many_args);
    RUN_TEST(proc_arguments_dont_leak_out);
    RUN_TEST(define_doesnt_leak_out);
    RUN_TEST(recursive_procedure);
    RUN_TEST(double_recursive_procedure);
    RUN_TEST(define_proc_inside_proc);
    RUN_TEST(define_proc_inside_proc_overrides_previous);
    RUN_TEST(good_enough_bug);
}


