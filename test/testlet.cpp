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

#include "testlet.h"

namespace
{

void let_sets_value()
{
    SubsInterpreter interpreter;
    interpreter.Interpret(
        "(define (fn)"
        "        (let ((x 5))"
        "             x));" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(fn)" ), "5" );
}

void value_is_unset_after()
{
    SubsInterpreter interpreter;
    interpreter.Interpret(
        "(define (fn)"
        "        (let ((x 5))"
        "             x)"
        "        x)" );

    TEST_ASSERT_THROWS_BEGIN
    {
        interpreter.Interpret( "(fn)" );
    }
    TEST_ASSERT_THROWS_END("Undefined symbol 'x'")
}


void let_sets_multiple_values()
{
    SubsInterpreter interpreter;
    interpreter.Interpret(
        "(define (fn)"
        "        (let ((x 5) (y 3))"
        "             (+ x y)));" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(fn)" ), "8" );
}


void can_calculate_values()
{
    SubsInterpreter interpreter;
    interpreter.Interpret(
        "(define (double x)"
        "        (let ((d (* x 2)))"
        "             d));" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(double 2)" ), "4" );
}


void can_recurse()
{
    SubsInterpreter interpreter;
    interpreter.Interpret(
        "(define (fact x)"
        "        (let ((y x))"
        "             (if (= y 1)"
        "                 1"
        "                 (* y (fact (- y 1))))));" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(fact 3)" ), "6" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(fact 4)" ), "24" );
}

void values_computed_outside_let()
{
    SubsInterpreter interpreter;

        // This value of x is what is used to define y later:
    interpreter.Interpret( "(define x 2)" );

    TEST_ASSERT_EQUAL(
        interpreter.Interpret(
            "(let ((x 3)"
            "      (y (+ x 2)))"
            "     (* x y))" ),
        "12" );
}


}

#define SUITENAME "TestLet"

void TestLet::Run() const
{
    RUN_TEST(let_sets_value);
    RUN_TEST(value_is_unset_after);
    RUN_TEST(let_sets_multiple_values);
    RUN_TEST(can_calculate_values);
    RUN_TEST(can_recurse);
    RUN_TEST(values_computed_outside_let);
}

