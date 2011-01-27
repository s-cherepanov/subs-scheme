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
#include "test/testquote.h"

using namespace std;

namespace
{


void symbol()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(quote a)" ), "a" );
}



void wrong_number_of_args_is_an_error()
{
    TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS( "quote", 1 );
}



void combination()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(quote (+ 1 2))" ),
        "(+ 1 2)" );
}


}

#define SUITENAME "TestQuote"

void TestQuote::Run() const
{
    RUN_TEST(symbol);
    RUN_TEST(wrong_number_of_args_is_an_error);
    RUN_TEST(combination);
}

