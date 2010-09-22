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

#include "testdisplay.h"

using namespace std;

namespace
{

void newline_writes_a_newline()
{
    SubsInterpreter interpreter;

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(newline)" ), "\n" );
}


void newline_with_any_args_is_an_error()
{
    TEST_ASSERT_THROWS_BEGIN
    {
        SubsInterpreter().Interpret( "(newline 4)" );
    }
    TEST_ASSERT_THROWS_END( "Too many operands to newline" );
}



void display_int_writes_an_int()
{
    SubsInterpreter interpreter;

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(display 5)" ), "5" );
}


void display_with_wrong_number_of_args_is_an_error()
{
    TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS( "display", 1 );
}

void display_string_writes_with_no_quotes()
{
    SubsInterpreter interpreter;

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(display \"foo\")" ), "foo" );
}


void display_symbol_evaluates_it_first()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define x 0.5)" );
    interpreter.Interpret( "(define y \"bar\")" );
    interpreter.Interpret( "(define (foo z) (display z))" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(display x)" ), "0.5" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(display y)" ), "bar" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(foo 3)" ), "3" );
}


}

#define SUITENAME "TestDisplay"

void TestDisplay::Run() const
{
    RUN_TEST(newline_writes_a_newline);
    RUN_TEST(newline_with_any_args_is_an_error);
    RUN_TEST(display_int_writes_an_int);
    RUN_TEST(display_with_wrong_number_of_args_is_an_error);
    RUN_TEST(display_string_writes_with_no_quotes);
    RUN_TEST(display_symbol_evaluates_it_first);
}

