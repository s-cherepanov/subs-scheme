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
#include "lib/evaluator.h"
#include "lib/lexer.h"
#include "lib/parser.h"
#include "lib/symbolvalue.h"
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

    TEST_ASSERT_EQUAL( lexer.NextToken().name, "" );
}


void create_symbolvalue()
{
    auto_ptr<Value> v( new SymbolValue( "+" ) );
}


void valuefactory_create_plus()
{
    ValueFactory::CreateValue( "+" );
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




}

void TestMemory::Run() const
{
    lex_empty_string();
    create_symbolvalue();
    valuefactory_create_plus();
    parse_emptystring();
    evaluate_null_value();
}

