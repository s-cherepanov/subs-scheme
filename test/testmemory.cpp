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

#include <memory>
#include <sstream>
#include <typeinfo>

#include "assertmacros.h"
#include "lib/customsymbolvalue.h"
#include "lib/evaluator.h"
#include "lib/integervalue.h"
#include "lib/lexer.h"
#include "lib/pairvalue.h"
#include "lib/parser.h"
#include "lib/stringvalue.h"
#include "lib/token.h"
#include "lib/valuefactory.h"

#include "testmemory.h"

using namespace std;

namespace
{

void lex_empty_string()
{
    istringstream ss( "" );
    Lexer lexer( ss );

    TEST_ASSERT_TRUE( lexer.NextToken().IsEndOfStream() );
}


void create_symbolvalue()
{
    auto_ptr<Value> v( new CustomSymbolValue( "+" ) );
}


void valuefactory_create_plus()
{
    Token token( "+", Token::eTypeNormal, 0 );
    ValueFactory::CreateValue( token );
}

void parse_emptystring()
{
    istringstream ss( "" );
    Lexer lexer( ss );
    Parser parser( lexer );
    std::auto_ptr<Value> parsed = parser.NextValue();

    TEST_ASSERT_NULL( parsed.get() );
}



void evaluate_null_value()
{
    ostringstream ss;
    std::auto_ptr<Value> evald = Evaluator().Eval( NULL, ss );

    TEST_ASSERT_NULL( evald.get() );
    TEST_ASSERT_EQUAL( ss.str(), "" );
}



void pair_create_clone_delete()
{
    std::auto_ptr<PairValue> pair;

    {
        std::auto_ptr<Value> i( new IntegerValue( 3 ) );
        std::auto_ptr<Value> s( new StringValue( "foo" ) );

        pair.reset( new PairValue( i, s ) );

        // i and s get deleted here
    }

    // We can clone it and no memory is leaked
    // TODO: Clone should return an auto_ptr
    std::auto_ptr<Value> clone( pair->Clone() );

    // The ones in the pair should be preserved
    const IntegerValue* iv = dynamic_cast<const IntegerValue*>(
        pair->GetFirst() );
    TEST_ASSERT_NOT_NULL( iv );
    int ii = iv->GetIntValue();
    TEST_ASSERT_EQUAL( ii, 3 );

    const StringValue* sv = dynamic_cast<const StringValue*>(
        pair->GetSecond() );
    TEST_ASSERT_NOT_NULL( sv );
    string sd = sv->GetStringValue();
    TEST_ASSERT_EQUAL( sd, "foo" );
}



}

#define SUITENAME "TestMemory"

void TestMemory::Run() const
{
    RUN_TEST(lex_empty_string);
    RUN_TEST(create_symbolvalue);
    RUN_TEST(valuefactory_create_plus);
    RUN_TEST(parse_emptystring);
    RUN_TEST(evaluate_null_value);
    RUN_TEST(pair_create_clone_delete);
}

