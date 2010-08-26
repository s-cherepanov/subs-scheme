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
#include "lib/bracketmatcher.h"
#include "lib/lexer.h"
#include "lib/tokenlist.h"

#include "testbracketmatcher.h"

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

    TEST_ASSERT_EQUAL( tokens.NextToken().name, "foo" );
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

    TEST_ASSERT_EQUAL( tokens.NextToken().name, "foo" );
    TEST_ASSERT_EQUAL( outss.str(), "" );
    TEST_ASSERT_TRUE( tokens.empty() );

    tokens = matcher.ReadCombination();

    TEST_ASSERT_EQUAL( tokens.NextToken().name, "bar" );
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

    TEST_ASSERT_EQUAL( tokens.NextToken().name, "(" );
    TEST_ASSERT_EQUAL( tokens.NextToken().name, "foo" );
    TEST_ASSERT_EQUAL( tokens.NextToken().name, "bar" );
    TEST_ASSERT_EQUAL( tokens.NextToken().name, ")" );
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

    TEST_ASSERT_EQUAL( tokens.NextToken().name, "(" );
    TEST_ASSERT_EQUAL( tokens.NextToken().name, "foo" );
    TEST_ASSERT_EQUAL( tokens.NextToken().name, "(" );
    TEST_ASSERT_EQUAL( tokens.NextToken().name, "bar" );
    TEST_ASSERT_EQUAL( tokens.NextToken().name, ")" );
    TEST_ASSERT_EQUAL( tokens.NextToken().name, "baz" );
    TEST_ASSERT_EQUAL( tokens.NextToken().name, ")" );
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

    TEST_ASSERT_EQUAL( tokens.NextToken().name, "(" );
    TEST_ASSERT_EQUAL( tokens.NextToken().name, "foo" );
    TEST_ASSERT_EQUAL( tokens.NextToken().name, "bar" );
    TEST_ASSERT_EQUAL( tokens.NextToken().name, "baz" );
    TEST_ASSERT_EQUAL( tokens.NextToken().name, ")" );
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

void TestBracketMatcher::Run() const
{
    empty_input_yields_empty_list();
    single_token_yields_one_item_list();
    multiple_tokens_yield_multiple_one_item_lists();
    combination_yields_all_tokens();
    nested_combination_yields_all_tokens();
    incomplete_combo_yields_empty_list();
    multiline_combo_yields_all_tokens_and_indents();
    multiline_multicombo_indents();
    // TODO: test passing false for print_prompt
}

