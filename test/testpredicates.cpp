/*
 *  Subs Scheme Interpreter
 *  Copyright (C) 2010, 2011 Andy Balaam
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
#include "test/testpredicates.h"

namespace
{

void or_nothing_is_false()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(or)" ), "#f" );
}


void or_one_thing()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(or #t)" ), "#t" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(or #f)" ), "#f" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(or 1)" ), "1" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(or (= 1 1))" ), "#t" );
}


void or_multiple_things()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(or #t #t #f)" ), "#t" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(or #f #f #t)" ), "#t" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(or #f #f #f)" ), "#f" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(or #f 1 3)" ), "1" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(or (= 1 3) (= 1 2))" ),
        "#f" );
}



void and_nothing_is_true()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(and)" ), "#t" );
}


void and_one_thing()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(and #t)" ), "#t" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(and #f)" ), "#f" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(and 1)" ), "1" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(and (= 1 1))" ), "#t" );
}


void and_multiple_things()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(and #t #t #f)" ), "#f" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(and #f #f #t)" ), "#f" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(and #t #t #t)" ), "#t" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(and #t 1 3)" ), "3" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(and (= 1 1) (= 2 1))" ),
        "#f" );
}


void wrong_number_of_args_to_not_is_an_error()
{
    TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS( "not", 1 )
}


void not_reverses_truthvalue()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(not #t)" ), "#f" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(not #f)" ), "#t" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(not (= 3 2))" ), "#t" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(not (= 1 1))" ), "#f" );
}


}

#define SUITENAME "TestPredicates"

void TestPredicates::Run() const
{
    RUN_TEST(or_nothing_is_false);
    RUN_TEST(or_one_thing);
    RUN_TEST(or_multiple_things);
    RUN_TEST(and_nothing_is_true);
    RUN_TEST(and_one_thing);
    RUN_TEST(and_multiple_things);
    RUN_TEST(wrong_number_of_args_to_not_is_an_error);
    RUN_TEST(not_reverses_truthvalue);
}

