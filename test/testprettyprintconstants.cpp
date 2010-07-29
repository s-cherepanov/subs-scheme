
#include "assertmacros.h"
#include "lib/combinationvalue.h"
#include "lib/integervalue.h"
#include "lib/prettyprinter.h"

#include "testprettyprintconstants.h"

using namespace std;

namespace
{

void int_value_simply_converted_to_string()
{
    IntegerValue value_5 ( 5 );

    TEST_ASSERT_EQUAL( PrettyPrinter::Print( &value_5 ), "5" );
}


void combination_written_as_bracketted_list()
{
    CombinationValue combo;
    combo.push_back( new IntegerValue( 5 ) );
    combo.push_back( new IntegerValue( 6 ) );
    combo.push_back( new IntegerValue( 7 ) );

    TEST_ASSERT_EQUAL( PrettyPrinter::Print( &combo ), "(5 6 7)" );
}



void combination_within_combination()
{
    CombinationValue combo;
    combo.push_back( new CombinationValue );
    CombinationValue* subcombo = static_cast<CombinationValue*>( combo.back() );
    subcombo->push_back( new IntegerValue( 3 ) );
    subcombo->push_back( new IntegerValue( 2 ) );
    subcombo->push_back( new IntegerValue( 1 ) );
    combo.push_back( new IntegerValue( 6 ) );
    combo.push_back( new IntegerValue( 7 ) );

    TEST_ASSERT_EQUAL( PrettyPrinter::Print( &combo ), "((3 2 1) 6 7)" );
}


}

void TestPrettyPrintConstants::Run() const
{
    int_value_simply_converted_to_string();
    combination_written_as_bracketted_list();
    combination_within_combination();
}

