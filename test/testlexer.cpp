
#include <memory>
#include <sstream>

#include "assertmacros.h"
#include "lib/newlexer.h"
#include "lib/token.h"

#include "testlexer.h"

using namespace std;

namespace
{

void empty_string_returns_empty_token()
{
    istringstream ss( "" );
    NewLexer lexer( ss );
    TEST_ASSERT_EQUAL( lexer.NextToken().name, "" );
}


void single_char()
{
    istringstream ss( "a" );
    NewLexer lexer( ss );
    TEST_ASSERT_EQUAL( lexer.NextToken().name, "a" );
}


void single_token()
{
    istringstream ss( "my-token" );
    NewLexer lexer( ss );
    TEST_ASSERT_EQUAL( lexer.NextToken().name, "my-token" );
}


void tokens_separated_by_space()
{
    istringstream ss( " 1 foo bar" );
    NewLexer lexer( ss );
    TEST_ASSERT_EQUAL( lexer.NextToken().name, "1" );
    TEST_ASSERT_EQUAL( lexer.NextToken().name, "foo" );
    TEST_ASSERT_EQUAL( lexer.NextToken().name, "bar" );
    TEST_ASSERT_EQUAL( lexer.NextToken().name, "" );

    istringstream ss2( "foo  3 bar a ");
    NewLexer lexer2( ss2 );
    TEST_ASSERT_EQUAL( lexer2.NextToken().name, "foo" );
    TEST_ASSERT_EQUAL( lexer2.NextToken().name, "3" );
    TEST_ASSERT_EQUAL( lexer2.NextToken().name, "bar" );
    TEST_ASSERT_EQUAL( lexer2.NextToken().name, "a" );
    TEST_ASSERT_EQUAL( lexer2.NextToken().name, "" );
}






}

void TestLexer::Run() const
{
    empty_string_returns_empty_token();
    single_char();
    single_token();
    tokens_separated_by_space();
}

