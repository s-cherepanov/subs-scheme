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

#include <sstream>

#include "lib/evaluationerror.h"
#include "lib/subsinterpreter.h"
#include "test/assertmacros.h"
#include "test/testbegin.h"

namespace
{

void runs_all_commands()
{
    std::ostringstream out;
    SubsInterpreter interpreter( out );
    interpreter.Interpret(
        "(define (fn)"
        "        (begin"
        "               (display 0)"
        "               (display 1)"
        "               (display 2)))" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(fn)" ), "012" );
}


void returns_last_return()
{
    SubsInterpreter interpreter;
    interpreter.Interpret(
        "(define (fn)"
        "        (begin"
        "               0"
        "               1"
        "               2"
        "               (+ 3 2)))" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(fn)" ), "5" );
}



void empty_is_ok()
{
    SubsInterpreter interpreter;
    // Does not crash
    interpreter.Interpret( "(begin)" );
}


void single_is_ok()
{
    SubsInterpreter interpreter;
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(begin 3)" ), "3" );
}



}

#define SUITENAME "TestBegin"

void TestBegin::Run() const
{
    RUN_TEST(runs_all_commands);
    RUN_TEST(returns_last_return);
    RUN_TEST(empty_is_ok);
    RUN_TEST(single_is_ok);
}

