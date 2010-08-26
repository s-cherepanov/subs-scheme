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

#include <sstream>

#include "assertmacros.h"
#include "lib/subsrepl.h"

#include "testrepl.h"

using namespace std;

namespace
{


#define REPL_TEST(input,exp_out) \
{ \
    istringstream in( input ); \
    ostringstream out; \
    ostringstream err; \
\
    int retval = SubsRepl( false ).Run( in, out, err ); \
\
    TEST_ASSERT_EQUAL( retval, 0 ); \
    TEST_ASSERT_EQUAL( err.str(), "" ); \
    TEST_ASSERT_EQUAL( out.str(), exp_out ); \
} \


void prompt_gets_written()
{
    istringstream in( "" );
    ostringstream out;
    ostringstream err;

    int retval = SubsRepl().Run( in, out, err );

    TEST_ASSERT_EQUAL( retval, 0 );
    TEST_ASSERT_EQUAL( err.str(), "" );
    TEST_ASSERT_EQUAL( out.str(), "> " );
}

void single_symbol_gets_evaluated()
{
    REPL_TEST( "5\n", "5\n" );
}


//void list_on_one_line_gets_evaluated_together()
//{
//    REPL_TEST( "(5 5)\n", "(5 5)\n" );
//}


void bare_undefined_symbol_reports_error()
{
    istringstream in( "foo" );
    ostringstream out;
    ostringstream err;

    int retval = SubsRepl( false ).Run( in, out, err );

    TEST_ASSERT_EQUAL( retval, 0 );
    TEST_ASSERT_CAN_FIND( err.str(), "Undefined symbol 'foo'" );
}


void undefined_symbol_operator_reports_error()
{
    istringstream in( "(foo 5 6)" );
    ostringstream out;
    ostringstream err;

    int retval = SubsRepl( false ).Run( in, out, err );

    TEST_ASSERT_EQUAL( retval, 0 );
    TEST_ASSERT_CAN_FIND( err.str(), "Undefined symbol 'foo'" );
}


void undefined_symbol_operand_reports_error()
{
    istringstream in( "(+ foo 6)" );
    ostringstream out;
    ostringstream err;

    int retval = SubsRepl( false ).Run( in, out, err );

    TEST_ASSERT_EQUAL( retval, 0 );
    TEST_ASSERT_CAN_FIND( err.str(), "Undefined symbol 'foo'" );
}


void unclosed_combination_indents()
{
    REPL_TEST( "(+ 1\n",
             ".    " );

    REPL_TEST( "(if 1\n",
             ".     " );

    REPL_TEST( "(if 1 (+ 4\n",
             ".          " );

    REPL_TEST( "(- 1233333\n"
                  "(if 41\n",
             ".    "
             ".        " );
}


void define_can_be_used_later()
{
    REPL_TEST( "(define foo 3)\nfoo\n",
             "foo\n3\n" );
}


void response_comes_even_when_stream_is_not_closed()
{
    // TODO: implement test that checks even when you don't close
    //       the stream the repl reads and processes the first line.
}


void pressing_return_emits_dot()
{
    istringstream in( "\n" );
    ostringstream out;
    ostringstream err;

    int retval = SubsRepl().Run( in, out, err );

    TEST_ASSERT_EQUAL( retval, 0 );
    TEST_ASSERT_EQUAL( err.str(), "" );
    TEST_ASSERT_EQUAL( out.str(), "> . " );
}



}

void TestRepl::Run() const
{
    prompt_gets_written();
    single_symbol_gets_evaluated();
    // INCORRECT could quote list_on_one_line_gets_evaluated_together();
    bare_undefined_symbol_reports_error();
    undefined_symbol_operator_reports_error();
    undefined_symbol_operand_reports_error();
    unclosed_combination_indents();
    define_can_be_used_later();
    response_comes_even_when_stream_is_not_closed();
    pressing_return_emits_dot();
}

