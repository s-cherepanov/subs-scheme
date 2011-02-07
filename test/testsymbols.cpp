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
#include "test/testsymbols.h"

using namespace std;

namespace
{


void symbolq_returns_true_for_quoted_symbol()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(symbol? 'foo)" ), "#t" );
}



void symbolq_takes_one_argument()
{
    TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS( "symbol?", 1 );
}


void symbolq_evaluates_argument()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(symbol? (car '(a b)))" ),
        "#t" );
}


void symbolq_returns_true_for_nil()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(symbol? 'nil)" ), "#t" );
}

void symbolq_returns_false_for_non_symbols()
{
    SubsInterpreter interpreter;

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(symbol? \"bar\")" ), "#f" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(symbol? '())" ), "#f" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(symbol? #f)" ), "#f" );
}



}

#define SUITENAME "TestSymbols"

void TestSymbols::Run() const
{
    RUN_TEST(symbolq_returns_true_for_quoted_symbol);
    RUN_TEST(symbolq_takes_one_argument);
    RUN_TEST(symbolq_evaluates_argument);
    RUN_TEST(symbolq_returns_true_for_nil);
    RUN_TEST(symbolq_returns_false_for_non_symbols);
}

