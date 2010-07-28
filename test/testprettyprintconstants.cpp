
#include "assertmacros.h"
#include "subs/integervalue.h"
#include "subs/prettyprinter.h"

#include "testprettyprintconstants.h"

using namespace std;

namespace
{

void int_value_simply_converted_to_string()
{
    IntegerValue value_5 ( 5 );

    TEST_ASSERT_EQUAL( PrettyPrinter::Print( &value_5 ), "5" );
}

}

void TestPrettyPrintConstants::Run() const
{
    int_value_simply_converted_to_string();
}

