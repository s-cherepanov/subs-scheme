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
#include "test/testcadr.h"

using namespace std;

namespace
{


void list()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define x (list 1 2 3 4 5))" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(car x)" ), "1" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(cadr x)" ), "2" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(caddr x)" ), "3" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(cadddr x)" ), "4" );

    TEST_ASSERT_THROWS_BEGIN
    {
        interpreter.Interpret( "(caddddr x)" );
    }
    TEST_ASSERT_THROWS_END( "Undefined symbol 'caddddr'" )
}


void tree()
{
    SubsInterpreter interpreter;

    interpreter.Interpret(
        "(define x (cons (cons (cons (cons \"llll\" \"lllr\")"
        "                            (cons \"llrl\" \"llrr\"))"
        "                      (cons (cons \"lrll\" \"lrlr\")"
        "                            (cons \"lrrl\" \"lrrr\")))"
        "                (cons (cons (cons \"rlll\" \"rllr\")"
        "                            (cons \"rlrl\" \"rlrr\"))"
        "                      (cons (cons \"rrll\" \"rrlr\")"
        "                            (cons \"rrrl\" \"rrrr\")))))" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(caaaar x)" ), "\"llll\"" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(cadaar x)" ), "\"llrl\"" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(caaddr x)" ), "\"rrll\"" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(cddddr x)" ), "\"rrrr\"" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(cadar x)" ),
        "(\"lrll\" . \"lrlr\")" );
}



void symbols()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define x (cons 10 20))" );

    interpreter.Interpret( "(define y (list 1 2 x))" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(cadr y)" ), "2" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(cdaddr y)" ), "20" );
}


}

#define SUITENAME "TestCadr"

void TestCadr::Run() const
{
    RUN_TEST(list);
    RUN_TEST(tree);
    RUN_TEST(symbols);
}

