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

#include <memory>
#include <string>

#include "lib/subsinterpreter.h"
#include "lib/value/basic/compoundprocedurevalue.h"
#include "test/assertmacros.h"
#include "test/evaldepthtracer.h"
#include "test/testtailcalloptimisation.h"

using namespace std;

namespace
{


void count_optimised()
{
    SubsInterpreter interpreter;

    EvalDepthTracer tracer;
    interpreter.SetTracer( &tracer );

    interpreter.Interpret(
        "(define (count n)"
        "        (if (= n 1)"
        "            1"
        "            (count (- n 1))))"
        );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(count 1)" ), "1" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(count 2)" ), "1" );

    interpreter.Interpret( "(count 10)" );

    // Even though Lisp recursed to 10 levels, the C++ stack never got beyond 3
    TEST_ASSERT_EQUAL( tracer.GetMaxEvalDepth(), 3 );
}


void bad_count_cant_be_optimised()
{
    SubsInterpreter interpreter;

    EvalDepthTracer tracer;
    interpreter.SetTracer( &tracer );

    interpreter.Interpret(
        "(define (badcount n)"
        "        (if (= n 1)"
        "            1"
        "            (badcount (- n 1)))"
        "        1)" // pointless statement - e.g. could be a print
        );

    interpreter.Interpret( "(badcount 10)" );

    // Because the tail-call optimisation is not available, the C++ stack
    // had to go to as many levels as the Lisp stack
    TEST_ASSERT_EQUAL( tracer.GetMaxEvalDepth(), 13 );
}


void and_count_optimised()
{
    SubsInterpreter interpreter;

    EvalDepthTracer tracer;
    interpreter.SetTracer( &tracer );

    interpreter.Interpret(
        "(define (andcount n)"
        "        (if (= n 1)"
        "            1"
        "            (and (> n 1)" // Unnecessary check that n > 1
        "                 (andcount (- n 1)))))"
        );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(andcount 1)" ), "1" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(andcount 2)" ), "1" );

    interpreter.Interpret( "(andcount 10)" );

    // Even though Lisp recursed to 10 levels, the C++ stack never got beyond 3
    TEST_ASSERT_EQUAL( tracer.GetMaxEvalDepth(), 3 );
}


void bad_and_count_cant_be_optimised()
{
    SubsInterpreter interpreter;

    EvalDepthTracer tracer;
    interpreter.SetTracer( &tracer );

    interpreter.Interpret(
        "(define (badandcount n)"
        "        (if (= n 1)"
        "            1"
        "            (and (badandcount (- n 1))" // order of 'and' args reversed
        "                 (> n 1))))"
        );

    interpreter.Interpret( "(badandcount 10)" );

    // Because the tail-call optimisation is not available, the C++ stack
    // had to go to as many levels as the Lisp stack
    TEST_ASSERT_EQUAL( tracer.GetMaxEvalDepth(), 12 );
}


void or_count_optimised()
{
    SubsInterpreter interpreter;

    EvalDepthTracer tracer;
    interpreter.SetTracer( &tracer );

    interpreter.Interpret(
        "(define (orcount n)"
        "        (if (= n 1)"
        "            1"
        "            (or (= n -1)" // Always-false check that n is -1
        "                (orcount (- n 1)))))"
        );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(orcount 1)" ), "1" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(orcount 2)" ), "1" );

    interpreter.Interpret( "(orcount 10)" );

    // Even though Lisp recursed to 10 levels, the C++ stack never got beyond 3
    TEST_ASSERT_EQUAL( tracer.GetMaxEvalDepth(), 3 );
}


void bad_or_count_cant_be_optimised()
{
    SubsInterpreter interpreter;

    EvalDepthTracer tracer;
    interpreter.SetTracer( &tracer );

    interpreter.Interpret(
        "(define (badorcount n)"
        "        (if (= n 1)"
        "            1"
        "            (or (badorcount (- n 1))" // order of 'or' args reversed
        "                (= n -1))))"
        );

    interpreter.Interpret( "(badorcount 10)" );

    // Because the tail-call optimisation is not available, the C++ stack
    // had to go to as many levels as the Lisp stack
    TEST_ASSERT_EQUAL( tracer.GetMaxEvalDepth(), 12 );
}


void count_with_let_optimised()
{
    SubsInterpreter interpreter;

    EvalDepthTracer tracer;
    interpreter.SetTracer( &tracer );

    interpreter.Interpret(
        "(define (count n)"
        "        (let ((x 0))"
        "             (if (= n 1)"
        "                 1"
        "                 (count (- n 1)))))"
        );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(count 1)" ), "1" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(count 2)" ), "1" );

    interpreter.Interpret( "(count 10)" );

    // Even though Lisp recursed to 10 levels, the C++ stack never got beyond 3
    TEST_ASSERT_EQUAL( tracer.GetMaxEvalDepth(), 3 );
}


void count_with_begin_optimised()
{
    SubsInterpreter interpreter;

    EvalDepthTracer tracer;
    interpreter.SetTracer( &tracer );

    interpreter.Interpret(
        "(define (count n)"
        "        (if (= n 1)"
        "            1"
        "            (begin"
        "                   1"
        "                   count (- n 1))))"
        );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(count 1)" ), "1" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(count 2)" ), "1" );

    interpreter.Interpret( "(count 10)" );

    // Even though Lisp recursed to 10 levels, the C++ stack never got beyond 3
    TEST_ASSERT_EQUAL( tracer.GetMaxEvalDepth(), 3 );
}



void count_with_begin_single_arg_optimised()
{
    SubsInterpreter interpreter;

    EvalDepthTracer tracer;
    interpreter.SetTracer( &tracer );

    interpreter.Interpret(
        "(define (count n)"
        "        (if (= n 1)"
        "            1"
        "            (begin"
        "                   count (- n 1))))"
        );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(count 1)" ), "1" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(count 2)" ), "1" );

    interpreter.Interpret( "(count 10)" );

    // Even though Lisp recursed to 10 levels, the C++ stack never got beyond 3
    TEST_ASSERT_EQUAL( tracer.GetMaxEvalDepth(), 3 );
}



void cdr_doesnt_prevent_optimisation()
{
    SubsInterpreter interpreter;

    interpreter.Interpret(
        "(define (mylength list) "
                "(define (length-iter total list) "
                        "(if (null? list) "
                            "total "
                            "(length-iter (+ total 1) (cdr list)))) "
                "(length-iter 0 list)) " );

    interpreter.Interpret(
        "(define mylist (list 0 1 2 3 4 5 6 7 8 9)) " );

    EvalDepthTracer tracer;
    interpreter.SetTracer( &tracer );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(mylength mylist)" ), "10" );

    // Even though Lisp recursed to 10 levels, the C++ stack never got beyond 3
    TEST_ASSERT_EQUAL( tracer.GetMaxEvalDepth(), 3 );
}



}

#define SUITENAME "TestTailCallOptimisation"

void TestTailCallOptimisation::Run() const
{
    RUN_TEST(count_optimised);
    RUN_TEST(bad_count_cant_be_optimised);
    RUN_TEST(and_count_optimised);
    RUN_TEST(bad_and_count_cant_be_optimised);
    RUN_TEST(or_count_optimised);
    RUN_TEST(bad_or_count_cant_be_optimised);
    RUN_TEST(count_with_let_optimised);
    RUN_TEST(count_with_begin_optimised);
    RUN_TEST(count_with_begin_single_arg_optimised);
    RUN_TEST(cdr_doesnt_prevent_optimisation);
}


