
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
    TEST_ASSERT_EQUAL( lexer.NextToken().name, "" );
}


void single_char()
{
    istringstream ss( "a" );
    Lexer lexer( ss );
    Token token = lexer.NextToken();

    TEST_ASSERT_EQUAL( token.name, "a" );
    TEST_ASSERT_EQUAL( token.column, 0 );
}


void single_token()
{
    istringstream ss( "my-token" );
    Lexer lexer( ss );
    TEST_ASSERT_EQUAL( lexer.NextToken().name, "my-token" );
}


void tokens_separated_by_space()
{
    istringstream ss( " 1 foo bar" );
    Lexer lexer( ss );
    Token token;

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.name, "1" );
    TEST_ASSERT_EQUAL( token.column, 1 );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.name, "foo" );
    TEST_ASSERT_EQUAL( token.column, 3 );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.name, "bar" );
    TEST_ASSERT_EQUAL( token.column, 7 );

    TEST_ASSERT_EQUAL( lexer.NextToken().name, "" );

    istringstream ss2( "foo  3 bar a ");
    Lexer lexer2( ss2 );

    token = lexer2.NextToken();
    TEST_ASSERT_EQUAL( token.name, "foo" );
    TEST_ASSERT_EQUAL( token.column, 0 );

    token = lexer2.NextToken();
    TEST_ASSERT_EQUAL( token.name, "3" );
    TEST_ASSERT_EQUAL( token.column, 5 );

    token = lexer2.NextToken();
    TEST_ASSERT_EQUAL( token.name, "bar" );
    TEST_ASSERT_EQUAL( token.column, 7 );

    token = lexer2.NextToken();
    TEST_ASSERT_EQUAL( token.name, "a" );
    TEST_ASSERT_EQUAL( token.column, 11 );

    TEST_ASSERT_EQUAL( lexer2.NextToken().name, "" );
}



void tokens_separated_by_newline()
{
    istringstream ss( " \n  1\n\n \n\nfoo\n \nbar baz \n\n" );
    Lexer lexer( ss );
    Token token;

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.name, "1" );
    TEST_ASSERT_EQUAL( token.column, 2 );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.name, "foo" );
    TEST_ASSERT_EQUAL( token.column, 0 );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.name, "bar" );
    TEST_ASSERT_EQUAL( token.column, 0 );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.name, "baz" );
    TEST_ASSERT_EQUAL( token.column, 4 );

    TEST_ASSERT_EQUAL( lexer.NextToken().name, "" );
}


void open_bracket_next_to_token()
{
    istringstream ss( "(if" );
    Lexer lexer( ss );
    Token token;

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.name, "(" );
    TEST_ASSERT_EQUAL( token.column, 0 );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.name, "if" );
    TEST_ASSERT_EQUAL( token.column, 1 );

    TEST_ASSERT_EQUAL( lexer.NextToken().name, "" );
}


void close_bracket_next_to_token()
{
    istringstream ss( "foo)" );
    Lexer lexer( ss );
    Token token;

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.name, "foo" );
    TEST_ASSERT_EQUAL( token.column, 0 );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.name, ")" );
    TEST_ASSERT_EQUAL( token.column, 3 );

    TEST_ASSERT_EQUAL( lexer.NextToken().name, "" );
}


void brackets_within_words()
{
    istringstream ss( "foo(bar)baz" );
    Lexer lexer( ss );
    Token token;

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.name, "foo" );
    TEST_ASSERT_EQUAL( token.column, 0 );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.name, "(" );
    TEST_ASSERT_EQUAL( token.column, 3 );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.name, "bar" );
    TEST_ASSERT_EQUAL( token.column, 4 );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.name, ")" );
    TEST_ASSERT_EQUAL( token.column, 7 );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.name, "baz" );
    TEST_ASSERT_EQUAL( token.column, 8 );

    TEST_ASSERT_EQUAL( lexer.NextToken().name, "" );
}



void brackets_on_their_own()
{
    istringstream ss( "\n)\n ) \n(\n )\n" );
    Lexer lexer( ss );
    Token token;

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.name, ")" );
    TEST_ASSERT_EQUAL( token.column, 0 );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.name, ")" );
    TEST_ASSERT_EQUAL( token.column, 1 );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.name, "(" );
    TEST_ASSERT_EQUAL( token.column, 0 );

    token = lexer.NextToken();
    TEST_ASSERT_EQUAL( token.name, ")" );
    TEST_ASSERT_EQUAL( token.column, 1 );

    TEST_ASSERT_EQUAL( lexer.NextToken().name, "" );
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
}

