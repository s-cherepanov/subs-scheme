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
#include "test/testcomparators.h"

namespace
{

void equals_works_for_integers()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(= 1 1)" ), "#t" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(= 1 2)" ), "#f" );

    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(= 1 1 1 1 1 1 1)" ),
        "#t" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(= 1 1 1 1 3 1 1)" ),
        "#f" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(= 3 1 1 1 1 1 1)" ),
        "#f" );
}


void too_few_operands_to_equals_is_an_error()
{
    TEST_ASSERT_THROWS_BEGIN
    {
        SubsInterpreter().Interpret( "(= 1)" );
    }
    TEST_ASSERT_THROWS_END( "Not enough" )
}


void lessthan_works_for_integers()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(< 1 1)" ), "#f" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(< 1 2)" ), "#t" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(< 2 1)" ), "#f" );

    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(< 2 4 6)" ), "#t" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(< 2 6 4)" ), "#f" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(< 6 2 4)" ), "#f" );
}


void lessthan_works_for_decimals()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(< 1.1 1.1)" ), "#f" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(< 1.1 1.5)" ), "#t" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(< 2.1 1)" ), "#f" );

    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(< 2.2 2.4 2.6)" ), "#t" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(< 2 6.6 4)" ), "#f" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(< 6 2.0 4)" ), "#f" );
}


void too_few_operands_to_lessthan_is_an_error()
{
    TEST_ASSERT_THROWS_BEGIN
    {
        SubsInterpreter().Interpret( "(< 1)" );
    }
    TEST_ASSERT_THROWS_END( "Not enough" )
}


void greaterthan_works_for_integers()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(> 1 1)" ), "#f" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(> 2 1)" ), "#t" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(> 1 2)" ), "#f" );

    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(> 6 4 2)" ), "#t" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(> 4 6 2)" ), "#f" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(> 4 2 6)" ), "#f" );
}


void greaterthan_works_for_decimals()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(> 1.4 1.4)" ), "#f" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(> 1.2 1.1)" ), "#t" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(> 1.0 2)" ), "#f" );

    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(> 1.6 1.4 1.2)" ), "#t" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(> 4 6.6 2)" ), "#f" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(> 4.2 2.1 6.9)" ), "#f" );
}


void too_few_operands_to_greaterthan_is_an_error()
{
    TEST_ASSERT_THROWS_BEGIN
    {
        SubsInterpreter().Interpret( "(> 1)" );
    }
    TEST_ASSERT_THROWS_END( "Not enough" )
}

}

#define SUITENAME "TestComparators"

void TestComparators::Run() const
{
    RUN_TEST(equals_works_for_integers);
    RUN_TEST(too_few_operands_to_equals_is_an_error);
    RUN_TEST(lessthan_works_for_integers);
    RUN_TEST(lessthan_works_for_decimals);
    RUN_TEST(too_few_operands_to_lessthan_is_an_error);
    RUN_TEST(greaterthan_works_for_integers);
    RUN_TEST(greaterthan_works_for_decimals);
    RUN_TEST(too_few_operands_to_greaterthan_is_an_error);
}

