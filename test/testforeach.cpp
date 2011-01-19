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

#include "lib/evaluationerror.h"
#include "lib/subsinterpreter.h"
#include "test/assertmacros.h"
#include "test/testforeach.h"

namespace
{


void runs_all_commands()
{
    SubsInterpreter interpreter;
    interpreter.Interpret(
        "(define (fn)"
        "        (for-each display (list 0 1 2)))" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(fn)" ), "012()" );
}

void wrong_num_args_is_an_error()
{
    TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS( "for-each", 2 );
}


void null_procedure_does_not_crash()
{
    // The return value of display can be tricky: ensure it works here
    TEST_ASSERT_THROWS_BEGIN
    {
        SubsInterpreter().Interpret( "(for-each (display 1) (list 0 1 2))" );
    }
    TEST_ASSERT_THROWS_END("Invalid");
}

void null_list_does_not_crash()
{
    TEST_ASSERT_THROWS_BEGIN
    {
        SubsInterpreter().Interpret( "(for-each display (display 1))" );
    }
    TEST_ASSERT_THROWS_END("Invalid");
}

void null_list_item_does_not_crash()
{
    // TODO: this should not throw!
    TEST_ASSERT_THROWS_BEGIN
    {
        SubsInterpreter().Interpret(
            "(for-each display (list 1 (display 1) 2))" );
    }
    TEST_ASSERT_THROWS_END("Invalid");
}




}

#define SUITENAME "TestForEach"

void TestForEach::Run() const
{
    RUN_TEST(runs_all_commands);
    RUN_TEST(wrong_num_args_is_an_error);
    RUN_TEST(null_procedure_does_not_crash);
    RUN_TEST(null_list_does_not_crash);
    RUN_TEST(null_list_item_does_not_crash);
    // NOT IMPLEMENTED: multiple list arguments
}

