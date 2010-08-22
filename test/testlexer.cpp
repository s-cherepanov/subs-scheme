
#include <memory>
#include <sstream>

#include "assertmacros.h"
#include "lib/newlexer.h"
#include "lib/token.h"

#include "testlexer.h"

using namespace std;

namespace
{

void empty_string_returns_empty_token()
{
    istringstream ss( "" );
    NewLexer lexer( ss );
    TEST_ASSERT_TRUE( lexer.NextToken().name.empty() );
}



}

void TestLexer::Run() const
{
    empty_string_returns_empty_token();
}

