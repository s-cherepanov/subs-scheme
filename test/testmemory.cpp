
#include <memory>
#include <sstream>
#include <typeinfo>

#include "assertmacros.h"
#include "lib/emptystringtree.h"
#include "lib/evaluator.h"
#include "lib/lexer.h"
#include "lib/parser.h"
#include "lib/newlexer.h"
#include "lib/newparser.h"
#include "lib/stringtreeleaf.h"
#include "lib/symbolvalue.h"
#include "lib/valuefactory.h"

#include "testmemory.h"

using namespace std;

namespace
{

void lex_empty_string()
{
    std::auto_ptr<StringTree> lexed = Lexer().Lex( "" );

    TEST_ASSERT_EQUAL( typeid( *lexed ), typeid( EmptyStringTree ) );
}


void create_symbolvalue()
{
    StringTreeLeaf leaf( "+" );
    auto_ptr<Value> v( new SymbolValue( leaf.str() ) );
}


void valuefactory_create_plus()
{
    StringTreeLeaf leaf( "+" );
    ValueFactory::CreateValue( &leaf );
}



void parse_emptystringtree_value()
{
    EmptyStringTree tree;
    std::auto_ptr<Value> parsed = Parser().Parse( &tree );

    TEST_ASSERT_NULL( parsed.get() );
}


void newparse_emptystringtree_value()
{
    istringstream ss( "" );
    NewLexer lexer( ss );
    NewParser parser( lexer );
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
    parse_emptystringtree_value();
    newparse_emptystringtree_value();
    evaluate_null_value();
}

