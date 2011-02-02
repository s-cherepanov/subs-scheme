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

#include "lib/bracketmatcher.h"
#include "lib/lexer.h"
#include "lib/tokenlist.h"
#include "test/assertmacros.h"
#include "test/testbracketmatcher.h"

using namespace std;

namespace
{


void empty_input_yields_empty_list()
{
    istringstream ss( "" );
    ostringstream outss;
    Lexer lexer( ss );
    BracketMatcher matcher( lexer, outss, true );

    TEST_ASSERT_TRUE( matcher.ReadCombination().empty() );

    TEST_ASSERT_EQUAL( outss.str(), "" );
}


void single_token_yields_one_item_list()
{
    istringstream ss( "foo" );
    ostringstream outss;
    Lexer lexer( ss );
    BracketMatcher matcher( lexer, outss, true );

    TokenList tokens = matcher.ReadCombination();

    TEST_ASSERT_EQUAL( tokens.NextToken().Name(), "foo" );
    TEST_ASSERT_EQUAL( outss.str(), "" );

    TEST_ASSERT_TRUE( tokens.empty() );
}

void multiple_tokens_yield_multiple_one_item_lists()
{
    istringstream ss( "foo bar" );
    ostringstream outss;
    Lexer lexer( ss );
    BracketMatcher matcher( lexer, outss, true );

    TokenList tokens = matcher.ReadCombination();

    TEST_ASSERT_EQUAL( tokens.NextToken().Name(), "foo" );
    TEST_ASSERT_EQUAL( outss.str(), "" );
    TEST_ASSERT_TRUE( tokens.empty() );

    tokens = matcher.ReadCombination();

    TEST_ASSERT_EQUAL( tokens.NextToken().Name(), "bar" );
    TEST_ASSERT_EQUAL( outss.str(), "" );
    TEST_ASSERT_TRUE( tokens.empty() );
}


void combination_yields_all_tokens()
{
    istringstream ss( "(foo bar)" );
    ostringstream outss;
    Lexer lexer( ss );
    BracketMatcher matcher( lexer, outss, true );

    TokenList tokens = matcher.ReadCombination();
    TEST_ASSERT_EQUAL( outss.str(), "" );

    TEST_ASSERT_EQUAL( tokens.NextToken().Name(), "(" );
    TEST_ASSERT_EQUAL( tokens.NextToken().Name(), "foo" );
    TEST_ASSERT_EQUAL( tokens.NextToken().Name(), "bar" );
    TEST_ASSERT_EQUAL( tokens.NextToken().Name(), ")" );
    TEST_ASSERT_TRUE( tokens.empty() );
}


void nested_combination_yields_all_tokens()
{
    istringstream ss( "(foo (bar) baz) po" );
    ostringstream outss;
    Lexer lexer( ss );
    BracketMatcher matcher( lexer, outss, true );

    TokenList tokens = matcher.ReadCombination();
    TEST_ASSERT_EQUAL( outss.str(), "" );

    TEST_ASSERT_EQUAL( tokens.NextToken().Name(), "(" );
    TEST_ASSERT_EQUAL( tokens.NextToken().Name(), "foo" );
    TEST_ASSERT_EQUAL( tokens.NextToken().Name(), "(" );
    TEST_ASSERT_EQUAL( tokens.NextToken().Name(), "bar" );
    TEST_ASSERT_EQUAL( tokens.NextToken().Name(), ")" );
    TEST_ASSERT_EQUAL( tokens.NextToken().Name(), "baz" );
    TEST_ASSERT_EQUAL( tokens.NextToken().Name(), ")" );
    TEST_ASSERT_TRUE( tokens.empty() );
}


void incomplete_combo_yields_empty_list()
{
    istringstream ss( "(foo (bar) baz" );
    ostringstream outss;
    Lexer lexer( ss );
    BracketMatcher matcher( lexer, outss, true );

    TokenList tokens = matcher.ReadCombination();

    TEST_ASSERT_EQUAL( outss.str(), "" );
    TEST_ASSERT_TRUE( tokens.empty() );
}



void multiline_combo_yields_all_tokens_and_indents()
{
    istringstream ss( "(foo bar\nbaz)" );
    ostringstream outss;
    Lexer lexer( ss );
    BracketMatcher matcher( lexer, outss, true );

    TokenList tokens = matcher.ReadCombination();
    TEST_ASSERT_EQUAL( outss.str(), ".      " );

    TEST_ASSERT_EQUAL( tokens.NextToken().Name(), "(" );
    TEST_ASSERT_EQUAL( tokens.NextToken().Name(), "foo" );
    TEST_ASSERT_EQUAL( tokens.NextToken().Name(), "bar" );
    TEST_ASSERT_EQUAL( tokens.NextToken().Name(), "baz" );
    TEST_ASSERT_EQUAL( tokens.NextToken().Name(), ")" );
    TEST_ASSERT_TRUE( tokens.empty() );
}


void multiline_multicombo_indents()
{
    istringstream ss( "(foo bar ( baz 4 \n)\n)" );
    ostringstream outss;
    Lexer lexer( ss );
    BracketMatcher matcher( lexer, outss, true );

    TokenList tokens = matcher.ReadCombination();
    TEST_ASSERT_EQUAL( outss.str(),
        ".                "
        ".      " );
}



}

#define SUITENAME "TestBracketMatcher"

void TestBracketMatcher::Run() const
{
    RUN_TEST(empty_input_yields_empty_list);
    RUN_TEST(single_token_yields_one_item_list);
    RUN_TEST(multiple_tokens_yield_multiple_one_item_lists);
    RUN_TEST(combination_yields_all_tokens);
    RUN_TEST(nested_combination_yields_all_tokens);
    RUN_TEST(incomplete_combo_yields_empty_list);
    RUN_TEST(multiline_combo_yields_all_tokens_and_indents);
    RUN_TEST(multiline_multicombo_indents);
    // TODO: test passing false for print_prompt
}

