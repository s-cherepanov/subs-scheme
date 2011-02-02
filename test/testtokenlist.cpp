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

#include "lib/tokenlist.h"
#include "test/assertmacros.h"
#include "test/testtokenlist.h"

using namespace std;

namespace
{


void empty_list_returns_empty_token()
{
    TokenList list;

    TEST_ASSERT_TRUE( list.NextToken().IsEndOfStream() );
    TEST_ASSERT_TRUE( list.NextToken().IsEndOfStream() );
}



void can_add_and_pop()
{
    TokenList list;

    Token token1( "foo", Token::eTypeNormal, 0 );
    list.AddToken( token1 );

    Token token2( "bar", Token::eTypeNormal, 0 );
    list.AddToken( token2 );

    TEST_ASSERT_EQUAL( list.NextToken().Name(), "foo" );
    TEST_ASSERT_EQUAL( list.NextToken().Name(), "bar" );
    TEST_ASSERT_TRUE( list.NextToken().IsEndOfStream() );
}



}

#define SUITENAME "TestTokenList"

void TestTokenList::Run() const
{
    RUN_TEST(empty_list_returns_empty_token);
    RUN_TEST(can_add_and_pop);
}

