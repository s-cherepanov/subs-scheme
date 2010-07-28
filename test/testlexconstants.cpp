
#include <memory>

#include "assertmacros.h"
#include "lib/lexer.h"
#include "lib/stringtreeleaf.h"

#include "testlexconstants.h"

using namespace std;

namespace
{

void bare_number_becomes_single_leaf()
{
    auto_ptr<StringTree> t = Lexer().Lex( "5" );

    StringTreeLeaf* lexed_5_leaf = dynamic_cast<StringTreeLeaf*>( t.get() );

    TEST_ASSERT_NOT_NULL( lexed_5_leaf );

    TEST_ASSERT_EQUAL( lexed_5_leaf->str(), "5" );
}

}

void TestLexConstants::Run() const
{
    bare_number_becomes_single_leaf();
}

