
#include <memory>
#include <sstream>

#include "assertmacros.h"
#include "lib/newlexer.h"

#include "testlexer.h"

using namespace std;

namespace
{

void empty_string_returns_null()
{
    istringstream ss( "" );
    NewLexer lexer( ss );
    TEST_ASSERT_NULL( lexer.NextToken().get() );
}



}

void TestLexer::Run() const
{
    empty_string_returns_null();
}

