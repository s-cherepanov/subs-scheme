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

#include <memory>
#include <sstream>

#include "assertmacros.h"
#include "lib/lexer.h"
#include "lib/token.h"

#include "testlexer.h"

using namespace std;

namespace
{

void empty_string_returns_empty_token()
{
    istringstream ss( "" );
    Lexer lexer( ss );
    TEST_ASSERT_EQUAL( lexer.NextToken().Name(), "" );
}


void single_char()
{
    istringstream ss( "a" );
    Lexer lexer( ss );
    Token token = lexer.NextToken();

    TEST_ASSERT_EQUAL( token.Name(), "a" );
    TEST_ASSERT_EQUAL( token.Column(), 0 );
}


void single_token()
{
    istringstream ss( "my-token" );
    Lexer lexer( ss );
    TEST_ASSERT_EQUAL( lexer.NextToken().Name(), "my-token" );
}


void tokens_separated_by_space()
{
    istringstream ss( " 1 foo bar" );
    Lexer lexer( ss );

    Token token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "1" );
    TEST_ASSERT_EQUAL( token.Column(), 1 );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "foo" );
    TEST_ASSERT_EQUAL( token.Column(), 3 );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "bar" );
    TEST_ASSERT_EQUAL( token.Column(), 7 );

    TEST_ASSERT_EQUAL( lexer.NextToken().Name(), "" );

    istringstream ss2( "foo  3 bar a ");
    Lexer lexer2( ss2 );

    token = lexer2.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "foo" );
    TEST_ASSERT_EQUAL( token.Column(), 0 );

    token = lexer2.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "3" );
    TEST_ASSERT_EQUAL( token.Column(), 5 );

    token = lexer2.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "bar" );
    TEST_ASSERT_EQUAL( token.Column(), 7 );

    token = lexer2.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "a" );
    TEST_ASSERT_EQUAL( token.Column(), 11 );

    TEST_ASSERT_EQUAL( lexer2.NextToken().Name(), "" );
}



void tokens_separated_by_newline()
{
    istringstream ss( " \n  1\n\n \n\nfoo\n \nbar baz \n\n" );
    Lexer lexer( ss );

    Token token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "1" );
    TEST_ASSERT_EQUAL( token.Column(), 2 );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "foo" );
    TEST_ASSERT_EQUAL( token.Column(), 0 );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "bar" );
    TEST_ASSERT_EQUAL( token.Column(), 0 );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "baz" );
    TEST_ASSERT_EQUAL( token.Column(), 4 );

    TEST_ASSERT_EQUAL( lexer.NextToken().Name(), "" );
}


void open_bracket_next_to_token()
{
    istringstream ss( "(if" );
    Lexer lexer( ss );

    Token token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "(" );
    TEST_ASSERT_EQUAL( token.Column(), 0 );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "if" );
    TEST_ASSERT_EQUAL( token.Column(), 1 );

    TEST_ASSERT_EQUAL( lexer.NextToken().Name(), "" );
}


void close_bracket_next_to_token()
{
    istringstream ss( "foo)" );
    Lexer lexer( ss );

    Token token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "foo" );
    TEST_ASSERT_EQUAL( token.Column(), 0 );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), ")" );
    TEST_ASSERT_EQUAL( token.Column(), 3 );

    TEST_ASSERT_EQUAL( lexer.NextToken().Name(), "" );
}


void brackets_within_words()
{
    istringstream ss( "foo(bar)baz" );
    Lexer lexer( ss );

    Token token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "foo" );
    TEST_ASSERT_EQUAL( token.Column(), 0 );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "(" );
    TEST_ASSERT_EQUAL( token.Column(), 3 );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "bar" );
    TEST_ASSERT_EQUAL( token.Column(), 4 );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), ")" );
    TEST_ASSERT_EQUAL( token.Column(), 7 );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "baz" );
    TEST_ASSERT_EQUAL( token.Column(), 8 );

    TEST_ASSERT_EQUAL( lexer.NextToken().Name(), "" );
}



void brackets_on_their_own()
{
    istringstream ss( "\n)\n ) \n(\n )\n" );
    Lexer lexer( ss );

    Token token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), ")" );
    TEST_ASSERT_EQUAL( token.Column(), 0 );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), ")" );
    TEST_ASSERT_EQUAL( token.Column(), 1 );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "(" );
    TEST_ASSERT_EQUAL( token.Column(), 0 );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), ")" );
    TEST_ASSERT_EQUAL( token.Column(), 1 );

    TEST_ASSERT_EQUAL( lexer.NextToken().Name(), "" );
}



void commented_line_is_ignored()
{
    istringstream ss( ";ignore all this" );
    Lexer lexer( ss );

    TEST_ASSERT_EQUAL( lexer.NextToken().Name(), "" );
}



void commented_after_line_with_space_is_ignored()
{
    istringstream ss( "(+ 1\n   2) ;ignore all this" );
    Lexer lexer( ss );

    TEST_ASSERT_EQUAL( lexer.NextToken().Name(), "(" );
    TEST_ASSERT_EQUAL( lexer.NextToken().Name(), "+" );
    TEST_ASSERT_EQUAL( lexer.NextToken().Name(), "1" );
    TEST_ASSERT_EQUAL( lexer.NextToken().Name(), "2" );
    TEST_ASSERT_EQUAL( lexer.NextToken().Name(), ")" );

    TEST_ASSERT_EQUAL( lexer.NextToken().Name(), "" );
}


void commented_after_line_no_space_is_ignored()
{
    istringstream ss( "(+ 1; ignore all this\n 2)" );
    Lexer lexer( ss );

    TEST_ASSERT_EQUAL( lexer.NextToken().Name(), "(" );
    TEST_ASSERT_EQUAL( lexer.NextToken().Name(), "+" );
    TEST_ASSERT_EQUAL( lexer.NextToken().Name(), "1" );
    TEST_ASSERT_EQUAL( lexer.NextToken().Name(), "2" );
    TEST_ASSERT_EQUAL( lexer.NextToken().Name(), ")" );

    TEST_ASSERT_EQUAL( lexer.NextToken().Name(), "" );
}






}

void TestLexer::Run() const
{
    empty_string_returns_empty_token();
    single_char();
    single_token();
    tokens_separated_by_space();
    tokens_separated_by_newline();
    open_bracket_next_to_token();
    close_bracket_next_to_token();
    brackets_within_words();
    brackets_on_their_own();
    commented_line_is_ignored();
    commented_after_line_with_space_is_ignored();
    commented_after_line_no_space_is_ignored();
}

