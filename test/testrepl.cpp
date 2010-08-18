
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
    TEST_ASSERT_EQUAL( out.str(), "> \n" );
}

void single_symbol_gets_evaluated()
{
    istringstream in( "5" );
    ostringstream out;
    ostringstream err;

    int retval = SubsRepl( false ).Run( in, out, err );

    TEST_ASSERT_EQUAL( retval, 0 );
    TEST_ASSERT_EQUAL( err.str(), "" );
    TEST_ASSERT_EQUAL( out.str(), "5\n" );
}


void list_on_one_line_gets_evaluated_together()
{
    istringstream in( "(5 5)" );
    ostringstream out;
    ostringstream err;

    int retval = SubsRepl( false ).Run( in, out, err );

    TEST_ASSERT_EQUAL( retval, 0 );
    TEST_ASSERT_EQUAL( err.str(), "" );
    TEST_ASSERT_EQUAL( out.str(), "(5 5)\n" );
}


void bare_undefined_symbol_reports_error()
{
    istringstream in( "foo" );
    ostringstream out;
    ostringstream err;

    int retval = SubsRepl( false ).Run( in, out, err );

    TEST_ASSERT_EQUAL( retval, 0 );
    TEST_ASSERT_CAN_FIND( err.str(), "Undefined symbol 'foo'" );
}


void undefined_symbol_operator_reports_error()
{
    istringstream in( "(foo 5 6)" );
    ostringstream out;
    ostringstream err;

    int retval = SubsRepl( false ).Run( in, out, err );

    TEST_ASSERT_EQUAL( retval, 0 );
    TEST_ASSERT_CAN_FIND( err.str(), "Undefined symbol 'foo'" );
}


void undefined_symbol_operand_reports_error()
{
    istringstream in( "(+ foo 6)" );
    ostringstream out;
    ostringstream err;

    int retval = SubsRepl( false ).Run( in, out, err );

    TEST_ASSERT_EQUAL( retval, 0 );
    TEST_ASSERT_CAN_FIND( err.str(), "Undefined symbol 'foo'" );
}




}

void TestRepl::Run() const
{
    prompt_gets_written();
    single_symbol_gets_evaluated();
    // INCORRECT could quote list_on_one_line_gets_evaluated_together();
    bare_undefined_symbol_reports_error();
    undefined_symbol_operator_reports_error();
    undefined_symbol_operand_reports_error();
}

