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
#include "test/testquestionfunctions.h"

using namespace std;

namespace
{

void wrong_num_args_to_pair_is_an_error()
{
    TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS( "pair?", 1 );
}


void pair_returns_true_if_pair()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define x (cons 2 3))" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(pair? x)" ), "#t" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(pair? 2)" ), "#f" );
}



void wrong_num_args_to_eq_is_an_error()
{
    TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS( "eq?", 2 );
}



void eq_returns_true_for_identical_symbols()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define x 'xx)" );
    interpreter.Interpret( "(define x1 'xx)" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(eq? 'xx 'xx)" ),         "#t" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(eq? 'define 'define)" ), "#t" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(eq? x 'xx)" ),           "#t" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(eq? x x1)" ),            "#t" );
}



void eq_returns_false_for_different_symbols()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define x 'xx)" );
    interpreter.Interpret( "(define y 'yy)" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(eq? 'aa 'bb)" ),   "#f" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(eq? x 'define)" ), "#f" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(eq? 'define x)" ), "#f" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(eq? x y)" ),       "#f" );
}





}

#define SUITENAME "TestQuestionFunctions"

void TestQuestionFunctions::Run() const
{
    RUN_TEST(wrong_num_args_to_pair_is_an_error);
    RUN_TEST(pair_returns_true_if_pair);
    RUN_TEST(wrong_num_args_to_eq_is_an_error)
    RUN_TEST(eq_returns_true_for_identical_symbols)
    RUN_TEST(eq_returns_false_for_different_symbols)
}

