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
    TEST_ASSERT_TRUE( lexer.NextToken().IsEndOfStream() );
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

    TEST_ASSERT_TRUE( lexer.NextToken().IsEndOfStream() );

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

    TEST_ASSERT_TRUE( lexer2.NextToken().IsEndOfStream() );
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

    TEST_ASSERT_TRUE( lexer.NextToken().IsEndOfStream() );
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

    TEST_ASSERT_TRUE( lexer.NextToken().IsEndOfStream() );
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

    TEST_ASSERT_TRUE( lexer.NextToken().IsEndOfStream() );
}


void brackets_within_words()
{
    istringstream ss( "foo(bar)baz" );
    Lexer lexer( ss );

    Token token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "foo" );
    TEST_ASSERT_EQUAL( token.Column(), 0 );
    TEST_ASSERT_EQUAL( token.Type(), Token::eTypeNormal );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "(" );
    TEST_ASSERT_EQUAL( token.Column(), 3 );
    TEST_ASSERT_EQUAL( token.Type(), Token::eTypeNormal );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "bar" );
    TEST_ASSERT_EQUAL( token.Column(), 4 );
    TEST_ASSERT_EQUAL( token.Type(), Token::eTypeNormal );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), ")" );
    TEST_ASSERT_EQUAL( token.Column(), 7 );
    TEST_ASSERT_EQUAL( token.Type(), Token::eTypeNormal );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "baz" );
    TEST_ASSERT_EQUAL( token.Column(), 8 );
    TEST_ASSERT_EQUAL( token.Type(), Token::eTypeNormal );

    TEST_ASSERT_TRUE( lexer.NextToken().IsEndOfStream() );
}



void brackets_on_their_own()
{
    istringstream ss( "\n)\n ) \n(\n )\n" );
    Lexer lexer( ss );

    Token token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), ")" );
    TEST_ASSERT_EQUAL( token.Column(), 0 );
    TEST_ASSERT_EQUAL( token.Type(), Token::eTypeNormal );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), ")" );
    TEST_ASSERT_EQUAL( token.Column(), 1 );
    TEST_ASSERT_EQUAL( token.Type(), Token::eTypeNormal );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "(" );
    TEST_ASSERT_EQUAL( token.Column(), 0 );
    TEST_ASSERT_EQUAL( token.Type(), Token::eTypeNormal );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), ")" );
    TEST_ASSERT_EQUAL( token.Column(), 1 );
    TEST_ASSERT_EQUAL( token.Type(), Token::eTypeNormal );

    TEST_ASSERT_TRUE( lexer.NextToken().IsEndOfStream() );
}



void commented_line_is_ignored()
{
    istringstream ss( ";ignore all this" );
    Lexer lexer( ss );

    TEST_ASSERT_TRUE( lexer.NextToken().IsEndOfStream() );
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

    TEST_ASSERT_TRUE( lexer.NextToken().IsEndOfStream() );
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

    TEST_ASSERT_TRUE( lexer.NextToken().IsEndOfStream() );
}


void simple_string_token()
{
    istringstream ss( "\"foo\"" );
    Lexer lexer( ss );

    Token token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "foo" );
    TEST_ASSERT_EQUAL( token.Column(), 0 );
    TEST_ASSERT_EQUAL( token.Type(), Token::eTypeString );

    TEST_ASSERT_TRUE( lexer.NextToken().IsEndOfStream() );
    // TODO: assert IsEndOfStream instead of name == "" everywhere
}


void string_amidst_other_tokens()
{
    istringstream ss( "(\"foo\" bar \"baz\" bish \"bosh\")" );
    Lexer lexer( ss );

    Token token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "(" );
    TEST_ASSERT_EQUAL( token.Column(), 0 );
    TEST_ASSERT_EQUAL( token.Type(), Token::eTypeNormal );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "foo" );
    TEST_ASSERT_EQUAL( token.Column(), 1 );
    TEST_ASSERT_EQUAL( token.Type(), Token::eTypeString );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "bar" );
    TEST_ASSERT_EQUAL( token.Column(), 7 );
    TEST_ASSERT_EQUAL( token.Type(), Token::eTypeNormal );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "baz" );
    TEST_ASSERT_EQUAL( token.Column(), 11 );
    TEST_ASSERT_EQUAL( token.Type(), Token::eTypeString );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "bish" );
    TEST_ASSERT_EQUAL( token.Column(), 17 );
    TEST_ASSERT_EQUAL( token.Type(), Token::eTypeNormal );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "bosh" );
    TEST_ASSERT_EQUAL( token.Column(), 22 );
    TEST_ASSERT_EQUAL( token.Type(), Token::eTypeString );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), ")" );
    TEST_ASSERT_EQUAL( token.Column(), 28 );
    TEST_ASSERT_EQUAL( token.Type(), Token::eTypeNormal );

    TEST_ASSERT_TRUE( lexer.NextToken().IsEndOfStream() );
}



void empty_string_token()
{
    istringstream ss( "\"\"" );
    Lexer lexer( ss );

    Token token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "" );
    TEST_ASSERT_EQUAL( token.Column(), 0 );
    TEST_ASSERT_EQUAL( token.Type(), Token::eTypeString );
    TEST_ASSERT_FALSE( token.IsEndOfStream() );

    token = lexer.NextToken();
    TEST_ASSERT_TRUE( token.IsEndOfStream() );
}



void string_adjacent_to_other_token()
{
    istringstream ss( "\"\"a \"boo\"\"goose\"daz x\"q\"" );
    Lexer lexer( ss );

    Token token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "" );
    TEST_ASSERT_EQUAL( token.Column(), 0 );
    TEST_ASSERT_EQUAL( token.Type(), Token::eTypeString );
    TEST_ASSERT_FALSE( token.IsEndOfStream() );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "a" );
    TEST_ASSERT_EQUAL( token.Column(), 2 );
    TEST_ASSERT_EQUAL( token.Type(), Token::eTypeNormal );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "boo" );
    TEST_ASSERT_EQUAL( token.Column(), 4 );
    TEST_ASSERT_EQUAL( token.Type(), Token::eTypeString );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "goose" );
    TEST_ASSERT_EQUAL( token.Column(), 9 );
    TEST_ASSERT_EQUAL( token.Type(), Token::eTypeString );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "daz" );
    TEST_ASSERT_EQUAL( token.Column(), 16 );
    TEST_ASSERT_EQUAL( token.Type(), Token::eTypeNormal );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "x" );
    TEST_ASSERT_EQUAL( token.Column(), 20 );
    TEST_ASSERT_EQUAL( token.Type(), Token::eTypeNormal );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "q" );
    TEST_ASSERT_EQUAL( token.Column(), 21 );
    TEST_ASSERT_EQUAL( token.Type(), Token::eTypeString );

    token = lexer.NextToken();
    TEST_ASSERT_TRUE( token.IsEndOfStream() );
}


void string_containing_space()
{
    istringstream ss( "\"foo bar\"" );
    Lexer lexer( ss );

    Token token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "foo bar" );
    TEST_ASSERT_EQUAL( token.Column(), 0 );
    TEST_ASSERT_EQUAL( token.Type(), Token::eTypeString );

    TEST_ASSERT_TRUE( lexer.NextToken().IsEndOfStream() );
    // TODO: assert IsEndOfStream instead of name == "" everywhere
}



void string_containing_bracket()
{
    istringstream ss( "\"(foo (bar) baz)\"" );
    Lexer lexer( ss );

    Token token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.Name(), "(foo (bar) baz)" );
    TEST_ASSERT_EQUAL( token.Column(), 0 );
    TEST_ASSERT_EQUAL( token.Type(), Token::eTypeString );

    TEST_ASSERT_TRUE( lexer.NextToken().IsEndOfStream() );
    // TODO: assert IsEndOfStream instead of name == "" everywhere
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
    simple_string_token();
    string_amidst_other_tokens();
    empty_string_token();
    string_adjacent_to_other_token();
    string_containing_space();
    string_containing_bracket();
    // NOTDONE: string_with_escaped_quote();
}

