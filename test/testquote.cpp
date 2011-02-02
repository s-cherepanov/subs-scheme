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
#include <sstream>

#include "lib/value/basic/combinationvalue.h"
#include "lib/evaluationerror.h"
#include "lib/lexer.h"
#include "lib/parser.h"
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


void literals()
{
    SubsInterpreter interpreter;

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(quote \"abc\")" ), "\"abc\"" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(quote 145932)" ),  "145932" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(quote #t)" ),      "#t" );
}


void in_function()
{
    SubsInterpreter interpreter;

    interpreter.Interpret(
        "(define (func)"
        "               (quote (* 4 (+ 2 1))))" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(func)" ), "(* 4 (+ 2 1))" );
}


void single_quote_form()
{
    SubsInterpreter interpreter;

    TEST_ASSERT_EQUAL( interpreter.Interpret( "'a" ), "a" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "'(+ 1 2)" ), "(+ 1 2)" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "'\"abc\"" ), "\"abc\"" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "'145932" ),  "145932" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "'#t" ),      "#t" );

    interpreter.Interpret(
        "(define (func)"
        "               '(* 4 (+ 2 1)))" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(func)" ), "(* 4 (+ 2 1))" );
}



void quoted_combo_acts_like_list()
{
    SubsInterpreter in;

    TEST_ASSERT_EQUAL( in.Interpret( "(car '((+ 2 3) 8))" ), "(+ 2 3)" );
    TEST_ASSERT_EQUAL( in.Interpret( "(car '(a b c))" ), "a" );
    TEST_ASSERT_EQUAL( in.Interpret( "(cdr '(a b c))" ), "(b c)" );
}



void quoted_empty_combo_parsed_correctly()
{
    istringstream ss( "'()" );
    Lexer lexer( ss );
    Parser parser( lexer );

    std::auto_ptr<Value> value = parser.NextValue();

    CombinationValue* combo = dynamic_cast<CombinationValue*>( value.get() );
    TEST_ASSERT_NOT_NULL( combo );
}



void quoted_empty_combo_acts_like_empty_list()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "'()" ), "()" );
}



void quoted_nested_combos_act_like_nested_lists()
{
    // Note: these 3 agree with mzscheme so they must be right :)

    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret(
        "(caaddr '(a b (+ 1 2) d))" ), "+" );

    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret(
        "(cadadr '(a (list b c)))" ), "b" );

    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret(
        "(cadadr '(a '(b c)))" ), "(b c)" );

}



void quote_within_quote_is_preserved()
{
    // TODO: render quotes with ' instead of (quote)
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret(
        "'(a b c '(d e f))" ), "(a b c (quote (d e f)))" );
}




}

#define SUITENAME "TestQuote"

void TestQuote::Run() const
{
    RUN_TEST(symbol);
    RUN_TEST(wrong_number_of_args_is_an_error);
    RUN_TEST(combination);
    RUN_TEST(literals);
    RUN_TEST(in_function);
    RUN_TEST(single_quote_form);
    RUN_TEST(quoted_combo_acts_like_list);
    RUN_TEST(quoted_empty_combo_parsed_correctly);
    RUN_TEST(quoted_empty_combo_acts_like_empty_list);
    RUN_TEST(quoted_nested_combos_act_like_nested_lists);
    RUN_TEST(quote_within_quote_is_preserved);
}

