
#include <sstream>

#include "assertmacros.h"
#include "lib/subsrepl.h"

#include "testrepl.h"

using namespace std;

namespace
{

void prompt_gets_written()
{
    istringstream in( "" );
    ostringstream out;
    ostringstream err;

    int retval = SubsRepl().Run( in, out, err );

    TEST_ASSERT_EQUAL( retval, 0 );
    TEST_ASSERT_EQUAL( err.str(), "" );
    TEST_ASSERT_EQUAL( out.str(), ">> " );
}

void single_symbol_gets_evaluated()
{
    istringstream in( "5" );
    ostringstream out;
    ostringstream err;

    int retval = SubsRepl( false ).Run( in, out, err );

    TEST_ASSERT_EQUAL( retval, 0 );
    TEST_ASSERT_EQUAL( err.str(), "" );
    TEST_ASSERT_EQUAL( out.str(), "5" );
}

}

void TestRepl::Run() const
{
    prompt_gets_written();
    single_symbol_gets_evaluated();
}

