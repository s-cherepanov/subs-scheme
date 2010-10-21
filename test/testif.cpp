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

#include "lib/evaluationerror.h"
#include "lib/subsinterpreter.h"
#include "test/assertmacros.h"
#include "test/testif.h"

using namespace std;

namespace
{

void true_and_false()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "#t" ), "#t" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "#f" ), "#f" );
}

void basic_if()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(if #t 10 20)" ), "10" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(if #f 10 20)" ), "20" );
}

void wrong_number_of_args_to_if_is_an_error()
{
    TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS( "if", 3 );
}

void everything_is_true_except_hash_f()
{
    SubsInterpreter interpreter;
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(if 0 6 7)" ), "6" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(if 1 6 7)" ), "6" );
    //FAILS TEST_ASSERT_EQUAL( interpreter.Interpret( "(if \"#t\" 6 7)" ),
    //    "6" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(if -1 6 7)" ), "6" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(if #f 6 7)" ), "7" );
}


void comparator_works_in_if()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret(
        "(if (= 3 4) 6 7)" ),
        "7" );

    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret(
        "(if (= 4 4) 6 7)" ),
        "6" );
}


}

#define SUITENAME "TestIf"

void TestIf::Run() const
{
    RUN_TEST(true_and_false);
    RUN_TEST(basic_if);
    RUN_TEST(wrong_number_of_args_to_if_is_an_error);
    RUN_TEST(everything_is_true_except_hash_f);
    RUN_TEST(comparator_works_in_if);
}

