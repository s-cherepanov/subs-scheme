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

#include "assertmacros.h"
#include "lib/combinationvalue.h"
#include "lib/integervalue.h"
#include "lib/prettyprinter.h"
#include "lib/stringvalue.h"

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

void string_written_in_quotes()
{
    StringValue value_foo_bar ( "foo bar" );

    TEST_ASSERT_EQUAL( PrettyPrinter::Print( &value_foo_bar ), "\"foo bar\"" );
}

}

#define SUITENAME "TestPrettyPrintConstants"

void TestPrettyPrintConstants::Run() const
{
    RUN_TEST(int_value_simply_converted_to_string);
    RUN_TEST(combination_written_as_bracketted_list);
    RUN_TEST(combination_within_combination);
    RUN_TEST(string_written_in_quotes);
    //NOTDONE: string_escapes_internal_quotes();
}

