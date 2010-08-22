
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
    std::auto_ptr<Value> evald = Evaluator().Eval( NULL );

    TEST_ASSERT_NULL( evald.get() );
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

