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

#include "testconstants.h"

namespace
{


void empty_string_yields_empty_string()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "" ), "" );
}


void bare_number_yields_itself()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "5" ), "5" );
}


void negative_number_yields_itself()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "-5" ), "-5" );
}


void decimal_number_yields_itself()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "1.45" ), "1.45" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( ".45" ), "0.45" );
}


void end_in_dot_is_failure()
{
    bool exception_caught = false;
    try
    {
        SubsInterpreter().Interpret( "1." );
    }
    catch( EvaluationError& )
    {
        exception_caught = true;
    }

    TEST_ASSERT_TRUE( exception_caught );
}



void two_dots_is_failure()
{
    bool exception_caught = false;
    try
    {
        SubsInterpreter().Interpret( "1.4.5" );
    }
    catch( EvaluationError& )
    {
        exception_caught = true;
    }

    TEST_ASSERT_TRUE( exception_caught );
}


void unparseable_integer_is_an_error()
{
    bool exception_caught = false;
    try
    {
        SubsInterpreter().Interpret( "10,000" );
    }
    catch( EvaluationError& )
    {
        exception_caught = true;
    }

    TEST_ASSERT_TRUE( exception_caught );
}


void decimal_equal_to_integer_ends_in_point_zero()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "2.0" ), "2.0" );
}





//void combo_yields_itself()
//{
//    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(4 5)" ), "(4 5)" );
//}
//
//
//
//void combo_of_combos_yields_itself()
//{
//    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(4 (5 4 1))" ),
//        "(4 (5 4 1))" );
//}



}

#define SUITENAME "TestConstants"

void TestConstants::Run() const
{
    RUN_TEST(empty_string_yields_empty_string);
    RUN_TEST(bare_number_yields_itself);
    RUN_TEST(negative_number_yields_itself);
    RUN_TEST(decimal_number_yields_itself);
    RUN_TEST(end_in_dot_is_failure);
    RUN_TEST(two_dots_is_failure);
    RUN_TEST(unparseable_integer_is_an_error);
    RUN_TEST(decimal_equal_to_integer_ends_in_point_zero);
    //INCORRECT could quote combo_yields_itself();
    //INCORRECT could quote combo_of_combos_yields_itself();
}

