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
#include "lib/evaluationerror.h"
#include "lib/subsinterpreter.h"

#include "testmaths.h"

using namespace std;

namespace
{

void log_no_args_is_an_error()
{
    bool exception_caught = false;
    try
    {
        SubsInterpreter().Interpret( "(log)" );
    }
    catch( EvaluationError& )
    {
        exception_caught = true;
    }
    TEST_ASSERT_TRUE( exception_caught );
}


void log_two_args_is_an_error()
{
    bool exception_caught = false;
    try
    {
        SubsInterpreter().Interpret( "(log 1 2)" );
    }
    catch( EvaluationError& )
    {
        exception_caught = true;
    }
    TEST_ASSERT_TRUE( exception_caught );
}

void log_calculates_ln_base_e()
{
    SubsInterpreter interpreter;
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(log 1)" ), "0.0" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(log 2)" ), "0.693147" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(log 10)" ), "2.30259" );
}

}

void TestMaths::Run() const
{
    log_no_args_is_an_error();
    log_two_args_is_an_error();
    log_calculates_ln_base_e();
}

