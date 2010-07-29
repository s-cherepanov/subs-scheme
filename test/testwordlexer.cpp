
#include <memory>

#include "assertmacros.h"
#include "lib/wordlexer.h"

#include "testwordlexer.h"

namespace
{

void strip_open_brackets_none()
{
    WordLexer wl;
    TEST_ASSERT_EQUAL( wl.HandleOpenBrackets( "56" ), "56" );
    TEST_ASSERT_EQUAL( wl.GetTreeStack().size(), 0 );
}

void strip_open_brackets_1()
{
    WordLexer wl;
    TEST_ASSERT_EQUAL( wl.HandleOpenBrackets( "(56" ), "56" );
    TEST_ASSERT_EQUAL( wl.GetTreeStack().size(), 1 );
}

void strip_open_brackets_4()
{
    WordLexer wl;
    TEST_ASSERT_EQUAL( wl.HandleOpenBrackets( "((((56" ), "56" );
    TEST_ASSERT_EQUAL( wl.GetTreeStack().size(), 4 );
}


void strip_close_brackets_none()
{
    unsigned int ret_num = 12;
    TEST_ASSERT_EQUAL( WordLexer().HandleCloseBrackets( "56", ret_num ), "56" );
    TEST_ASSERT_EQUAL( ret_num, 0 );
}

void strip_close_brackets_1()
{
    unsigned int ret_num = 12;
    TEST_ASSERT_EQUAL( WordLexer().HandleCloseBrackets( "56)", ret_num ),
        "56" );
    TEST_ASSERT_EQUAL( ret_num, 1 );
}

void strip_close_brackets_4()
{
    unsigned int ret_num = 12;
    TEST_ASSERT_EQUAL( WordLexer().HandleCloseBrackets( "56))))", ret_num ),
        "56" );
    TEST_ASSERT_EQUAL( ret_num, 4 );
}





}

void TestWordLexer::Run() const
{
    strip_open_brackets_none();
    strip_open_brackets_1();
    strip_open_brackets_4();
    strip_close_brackets_none();
    strip_close_brackets_1();
    strip_close_brackets_4();
}

