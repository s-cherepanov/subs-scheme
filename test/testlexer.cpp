
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




}

void TestLexer::Run() const
{
    empty_string_returns_empty_token();
    single_char();
    single_token();
}

