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

#include <string>

#include "lib/evaluationerror.h"
#include "lib/subsinterpreter.h"
#include "test/assertmacros.h"
#include "test/testcons.h"

using namespace std;

namespace
{

void wrong_num_args_is_an_error()
{
    TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS( "cons", 2 );
    TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS( "car", 1 );
    TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS( "cdr", 1 );
}


void cons_creates_a_pair()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define x (cons 2 3))" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(car x)" ), "2" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(cdr x)" ), "3" );
}



void can_nest_pairs()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define x (cons (cons 1 2) 3))" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(car (car x))" ), "1" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(cdr (car x))" ), "2" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(cdr x)" ), "3" );
}


void car_requires_a_pair()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define x 1)" );

    TEST_ASSERT_THROWS_BEGIN
    {
        interpreter.Interpret( "(car x)" );
    }
    TEST_ASSERT_THROWS_END( "The argument to car must be a pair. '1' is not" )
}



void cdr_requires_a_pair()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define x 1)" );

    TEST_ASSERT_THROWS_BEGIN
    {
        interpreter.Interpret( "(cdr x)" );
    }
    TEST_ASSERT_THROWS_END( "The argument to cdr must be a pair. '1' is not" )
}




}

#define SUITENAME "TestCons"

void TestCons::Run() const
{
    RUN_TEST(wrong_num_args_is_an_error);
    RUN_TEST(cons_creates_a_pair);
    RUN_TEST(can_nest_pairs);
    RUN_TEST(car_requires_a_pair);
    RUN_TEST(cdr_requires_a_pair);
}

