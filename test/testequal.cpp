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

#include "assertmacros.h"
#include "lib/decimalvalue.h"
#include "lib/integervalue.h"
#include "lib/subsinterpreter.h"

#include "testequal.h"

using namespace std;

namespace
{

void ints()
{
    SubsInterpreter interpreter;

    TEST_ASSERT_EQUAL(interpreter.Interpret( "(equal? 5 5)" ), "#t" );
    TEST_ASSERT_EQUAL(interpreter.Interpret( "(equal? 1 2)" ), "#f" );
    TEST_ASSERT_EQUAL(interpreter.Interpret( "(equal? 1 -1)" ), "#f" );
}



void decimals()
{
    SubsInterpreter interpreter;

    TEST_ASSERT_EQUAL(interpreter.Interpret( "(equal? 5.2 5.1)" ), "#f" );
    TEST_ASSERT_EQUAL(interpreter.Interpret( "(equal? 5.2 5.2)" ), "#t" );
    TEST_ASSERT_EQUAL(interpreter.Interpret( "(equal? 5.4 6.4)" ), "#f" );
    TEST_ASSERT_EQUAL(interpreter.Interpret( "(equal? 0.1 -0.1)" ), "#f" );

    // Note: these 2 come out false in mzscheme, possibly because of exactness?
    // Really, from R5RS, I think they should be true.
    TEST_ASSERT_EQUAL(interpreter.Interpret( "(equal? 3.0 3)" ), "#f" );
    TEST_ASSERT_EQUAL(interpreter.Interpret( "(equal? 4 4.0)" ), "#f" );
}


}

#define SUITENAME "TestEqual"

void TestEqual::Run() const
{
    RUN_TEST(ints);
    RUN_TEST(decimals);
    // TODO: RUN_TEST(strings);
    // TODO: RUN_TEST(pairs);
    // TODO: RUN_TEST(lists);
    // TODO: RUN_TEST(nativefunctions);
    // TODO: RUN_TEST(compoundprocedures);
}

