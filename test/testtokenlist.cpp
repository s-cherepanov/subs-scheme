
#include "assertmacros.h"
#include "lib/tokenlist.h"

#include "testtokenlist.h"

using namespace std;

namespace
{


void empty_list_returns_empty_token()
{
    TokenList list;

    TEST_ASSERT_EQUAL( list.NextToken().name, "" );
    TEST_ASSERT_EQUAL( list.NextToken().name, "" );
}



void can_add_and_pop()
{
    TokenList list;

    Token token;
    token.name = "foo";
    list.AddToken( token );

    token.name = "bar";
    list.AddToken( token );

    TEST_ASSERT_EQUAL( list.NextToken().name, "foo" );
    TEST_ASSERT_EQUAL( list.NextToken().name, "bar" );
    TEST_ASSERT_EQUAL( list.NextToken().name, "" );
}



}

void TestTokenList::Run() const
{
    empty_list_returns_empty_token();
    can_add_and_pop();
}

