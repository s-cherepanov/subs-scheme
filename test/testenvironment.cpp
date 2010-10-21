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

#include "lib/value/basic/integervalue.h"
#include "lib/environment.h"
#include "test/assertmacros.h"
#include "test/testenvironment.h"

using namespace std;

namespace
{


void insert_symbol()
{
    Environment().InsertSymbol( "foo", new IntegerValue( 3 ) );
}


void find_symbol()
{
    IntegerValue* intval = new IntegerValue( 3 );
    Environment env;
    env.InsertSymbol( "foo", intval );

    const Value* found = env.FindSymbol( "foo" );

    TEST_ASSERT_EQUAL( found, intval );
}



void insert_symbol_twice()
{
    IntegerValue* val3 = new IntegerValue( 3 );
    IntegerValue* val4 = new IntegerValue( 4 );

    Environment env;
    env.InsertSymbol( "foo", val3 );
    env.InsertSymbol( "foo", val4 ); // 3 should be deleted here

    const Value* found = env.FindSymbol( "foo" );

    TEST_ASSERT_EQUAL( found, val4 );
}





}

#define SUITENAME "TestEnvironment"

void TestEnvironment::Run() const
{
    RUN_TEST(insert_symbol);
    RUN_TEST(find_symbol);
    RUN_TEST(insert_symbol_twice);
}

