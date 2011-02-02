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

#include <sstream>

#include "lib/subsrepl.h"
#include "test/assertmacros.h"
#include "test/testrepl.h"

using namespace std;

namespace
{


#define REPL_TEST(input,exp_out) \
{ \
    istringstream in( input ); \
    ostringstream out; \
    ostringstream err; \
\
    int retval = SubsRepl( out, SubsRepl::ePrintContinuation ).Run( in, err ); \
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

    int retval = SubsRepl( out, SubsRepl::ePrintPrompt ).Run( in, err );

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

    SubsRepl( out, SubsRepl::eNoResponse ).Run( in, err );

    TEST_ASSERT_CAN_FIND( err.str(), "Undefined symbol 'foo'" );
}


void undefined_symbol_operator_reports_error()
{
    istringstream in( "(foo 5 6)" );
    ostringstream out;
    ostringstream err;

    SubsRepl( out, SubsRepl::eNoResponse ).Run( in, err );

    TEST_ASSERT_CAN_FIND( err.str(), "Undefined symbol 'foo'" );
}


void undefined_symbol_operand_reports_error()
{
    istringstream in( "(+ foo 6)" );
    ostringstream out;
    ostringstream err;

    SubsRepl( out, SubsRepl::eNoResponse ).Run( in, err );

    TEST_ASSERT_CAN_FIND( err.str(), "Undefined symbol 'foo'" );
}



void continues_after_error()
{
    istringstream in( "(+ foo 6)\n(* 6 7)" );
    ostringstream out;
    ostringstream err;

    SubsRepl( out, SubsRepl::eNoResponse ).Run( in, err );

    TEST_ASSERT_CAN_FIND( err.str(), "Undefined symbol 'foo'" );
    TEST_ASSERT_CAN_FIND( out.str(), "42" );
    // TODO: check for spurious dots - should not be printed after error
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

    int retval = SubsRepl( out,
        SubsRepl::ePrintPrompt | SubsRepl::ePrintContinuation ).Run( in, err );

    TEST_ASSERT_EQUAL( retval, 0 );
    TEST_ASSERT_EQUAL( err.str(), "" );
    TEST_ASSERT_EQUAL( out.str(), "> . " );
}



}

#define SUITENAME "TestRepl"

void TestRepl::Run() const
{
    RUN_TEST(prompt_gets_written);
    RUN_TEST(single_symbol_gets_evaluated);
    // INCORRECT could quote list_on_one_line_gets_evaluated_together();
    RUN_TEST(bare_undefined_symbol_reports_error);
    RUN_TEST(undefined_symbol_operator_reports_error);
    RUN_TEST(undefined_symbol_operand_reports_error);
    RUN_TEST(continues_after_error);
    RUN_TEST(unclosed_combination_indents);
    RUN_TEST(define_can_be_used_later);
    RUN_TEST(response_comes_even_when_stream_is_not_closed);
    RUN_TEST(pressing_return_emits_dot);
}

