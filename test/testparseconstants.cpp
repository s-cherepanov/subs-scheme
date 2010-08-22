
#include <memory>
#include <sstream>

#include "assertmacros.h"
#include "lib/combinationvalue.h"
#include "lib/integervalue.h"
#include "lib/lexer.h"
#include "lib/parser.h"
#include "lib/parsingerror.h"
#include "lib/symbolvalue.h"
#include "lib/unfinishedcombinationexception.h"
#include "lib/value.h"

#include "testparseconstants.h"

using namespace std;

namespace
{

void bare_number_becomes_single_value()
{
    istringstream ss( "5" );
    Lexer lexer( ss );
    Parser parser( lexer );
    auto_ptr<Value> v = parser.NextValue();

    IntegerValue* parsed_5 = dynamic_cast<IntegerValue*>( v.get() );

    TEST_ASSERT_NOT_NULL( parsed_5 );

    TEST_ASSERT_EQUAL( parsed_5->GetIntValue(), 5 );

    TEST_ASSERT_NULL( parser.NextValue().get() );
}


void branch_becomes_combination_value()
{
    istringstream ss( "(31 32 33)" );
    Lexer lexer( ss );
    Parser parser( lexer );

    auto_ptr<Value> v = parser.NextValue();

    CombinationValue* parsed_combo = dynamic_cast<CombinationValue*>( v.get() );

    TEST_ASSERT_NOT_NULL( parsed_combo );

    TEST_ASSERT_EQUAL( parsed_combo->size(), 3 );

    IntegerValue* val = dynamic_cast<IntegerValue*>( (*parsed_combo)[0] );
    TEST_ASSERT_NOT_NULL( val );
    TEST_ASSERT_EQUAL( val->GetIntValue(), 31 );

    val = dynamic_cast<IntegerValue*>( (*parsed_combo)[1] );
    TEST_ASSERT_NOT_NULL( val );
    TEST_ASSERT_EQUAL( val->GetIntValue(), 32 );

    val = dynamic_cast<IntegerValue*>( (*parsed_combo)[2] );
    TEST_ASSERT_NOT_NULL( val );
    TEST_ASSERT_EQUAL( val->GetIntValue(), 33 );

    TEST_ASSERT_NULL( parser.NextValue().get() );
}


void name_becomes_symbol_value()
{
    istringstream ss( "foo" );
    Lexer lexer( ss );
    Parser parser( lexer );

    auto_ptr<Value> v = parser.NextValue();

    SymbolValue* parsed_foo = dynamic_cast<SymbolValue*>( v.get() );

    TEST_ASSERT_NOT_NULL( parsed_foo );

    TEST_ASSERT_EQUAL( parsed_foo->GetStringValue(), "foo" );

    TEST_ASSERT_NULL( parser.NextValue().get() );
}


void plus_becomes_symbol_value()
{
    istringstream ss( "+" );
    Lexer lexer( ss );
    Parser parser( lexer );

    auto_ptr<Value> v = parser.NextValue();

    SymbolValue* parsed = dynamic_cast<SymbolValue*>( v.get() );

    TEST_ASSERT_NOT_NULL( parsed );

    TEST_ASSERT_EQUAL( parsed->GetStringValue(), "+" );

    TEST_ASSERT_NULL( parser.NextValue().get() );
}


void unclosed_bracket_throws()
{
    istringstream ss( "(if 1" );
    Lexer lexer( ss );
    Parser parser( lexer );

    bool exception_caught = false;
    try
    {
        parser.NextValue();
    }
    catch( UnfinishedCombinationException& )
    {
        exception_caught = true;
    }

    TEST_ASSERT_TRUE( exception_caught );
}



void too_many_close_brackets_throws()
{
    istringstream ss( "(if 1 3) 4)" );
    Lexer lexer( ss );
    Parser parser( lexer );

    bool exception_caught = false;
    try
    {
        // Call NextValue until we finally hit the close bracket
        parser.NextValue();
        parser.NextValue();
        parser.NextValue();
    }
    catch( ParsingError& )
    {
        exception_caught = true;
    }

    TEST_ASSERT_TRUE( exception_caught );
}



}

void TestParseConstants::Run() const
{
    bare_number_becomes_single_value();
    branch_becomes_combination_value();
    name_becomes_symbol_value();
    plus_becomes_symbol_value();
    unclosed_bracket_throws();
    too_many_close_brackets_throws();
}

