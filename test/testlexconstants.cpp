
#include <memory>

#include "assertmacros.h"
#include "lib/lexer.h"
#include "lib/stringtreebranch.h"
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


void combination_becomes_branch_with_leaves()
{
    auto_ptr<StringTree> t = Lexer().Lex( "(5 6 7)" );

    StringTreeBranch* lexed_branch = dynamic_cast<StringTreeBranch*>( t.get() );

    TEST_ASSERT_NOT_NULL( lexed_branch );

    TEST_ASSERT_EQUAL( lexed_branch->size(), 3 );

    const char* expected[] = { "5", "6", "7" };

    for( unsigned int i = 0; i < 3; ++i )
    {
        StringTreeLeaf* lexed_leaf = dynamic_cast<StringTreeLeaf*>(
            (*lexed_branch)[i] );

        TEST_ASSERT_NOT_NULL( lexed_leaf );

        TEST_ASSERT_EQUAL( lexed_leaf->str(), expected[i] );
    }
}


void combination_containing_combination()
{
    auto_ptr<StringTree> t = Lexer().Lex( "(5 (6 3) 7)" );

    StringTreeBranch* lexed_branch = dynamic_cast<StringTreeBranch*>( t.get() );

    TEST_ASSERT_NOT_NULL( lexed_branch );

    TEST_ASSERT_EQUAL( lexed_branch->size(), 3 );

    StringTreeLeaf* lexed_leaf = dynamic_cast<StringTreeLeaf*>(
        (*lexed_branch)[0] );
    TEST_ASSERT_NOT_NULL( lexed_leaf );
    TEST_ASSERT_EQUAL( lexed_leaf->str(), "5" );

    lexed_leaf = dynamic_cast<StringTreeLeaf*>( (*lexed_branch)[2] );
    TEST_ASSERT_NOT_NULL( lexed_leaf );
    TEST_ASSERT_EQUAL( lexed_leaf->str(), "7" );

    lexed_branch = dynamic_cast<StringTreeBranch*>( (*lexed_branch)[1] );
    TEST_ASSERT_NOT_NULL( lexed_branch );
    TEST_ASSERT_EQUAL( lexed_branch->size(), 2 );

    lexed_leaf = dynamic_cast<StringTreeLeaf*>( (*lexed_branch)[0] );
    TEST_ASSERT_NOT_NULL( lexed_leaf );
    TEST_ASSERT_EQUAL( lexed_leaf->str(), "6" );

    lexed_leaf = dynamic_cast<StringTreeLeaf*>( (*lexed_branch)[1] );
    TEST_ASSERT_NOT_NULL( lexed_leaf );
    TEST_ASSERT_EQUAL( lexed_leaf->str(), "3" );
}



}

void TestLexConstants::Run() const
{
    bare_number_becomes_single_leaf();
    combination_becomes_branch_with_leaves();
    combination_containing_combination();
}

