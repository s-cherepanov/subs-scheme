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

#include "testif.h"

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

void too_few_args_to_if_is_an_error()
{
    bool exception_caught = false;
    try
    {
        SubsInterpreter().Interpret( "(if #t 1)" );
    }
    catch( EvaluationError& e )
    {
        TEST_ASSERT_CAN_FIND( e.ToString(), "Not enough" );
        exception_caught = true;
    }

    TEST_ASSERT_TRUE( exception_caught );
}

void too_many_args_to_if_is_an_error()
{
    bool exception_caught = false;
    try
    {
        SubsInterpreter().Interpret( "(if #t 1 2 3)" );
    }
    catch( EvaluationError& e )
    {
        TEST_ASSERT_CAN_FIND( e.ToString(), "Too many" );
        exception_caught = true;
    }

    TEST_ASSERT_TRUE( exception_caught );
}

void everything_is_true_except_hash_f()
{
    SubsInterpreter interpreter;
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(if 0 6 7)" ), "6" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(if 1 6 7)" ), "6" );
    //FAILS TEST_ASSERT_EQUAL( interpreter.Interpret( "(if \"#t\" 6 7)" ),
    //    "6" );
    //FAILS TEST_ASSERT_EQUAL( interpreter.Interpret( "(if -1 6 7)" ), "6" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(if #f 6 7)" ), "7" );
}


}

void TestIf::Run() const
{
    true_and_false();
    basic_if();
    too_few_args_to_if_is_an_error();
    too_many_args_to_if_is_an_error();
    everything_is_true_except_hash_f();
    //NOTDONE comparator_works_in_if();
}

