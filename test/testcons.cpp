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

#include "testcons.h"

using namespace std;

namespace
{

void wrong_num_args_is_an_error()
{
    TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS( "cons", 2 );
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


void list_creates_nested_pairs()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define x (list 1 2 3))" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(car x)" ), "1" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(cdr x)" ), "(2 3)" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(car (cdr x))" ), "2" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(car (cdr (cdr x)))" ), "3" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(cdr (cdr (cdr x)))" ), "()" );
}



}

#define SUITENAME "TestCons"

void TestCons::Run() const
{
    RUN_TEST(wrong_num_args_is_an_error);
    RUN_TEST(cons_creates_a_pair);
    RUN_TEST(can_nest_pairs);
    RUN_TEST(list_creates_nested_pairs);
}

