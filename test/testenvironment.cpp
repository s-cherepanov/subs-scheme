
#include "assertmacros.h"
#include "lib/environment.h"
#include "lib/integervalue.h"

#include "testenvironment.h"

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

void TestEnvironment::Run() const
{
    insert_symbol();
    find_symbol();
    insert_symbol_twice();
}

