
#include <iostream>
#include <memory>

#include "newlexer.h"
#include "token.h"

using namespace std;

NewLexer::NewLexer( std::istream& instream )
: instream_( instream )
{
}

std::auto_ptr<Token> NewLexer::NextToken()
{
    if( !instream_.eof() )
    {
        return std::auto_ptr<Token>( NULL );
    }

    auto_ptr<Token> ret( new Token );

    return ret;
}

