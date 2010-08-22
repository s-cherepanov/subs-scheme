
#include <iostream>

#include "newlexer.h"
#include "token.h"

NewLexer::NewLexer( std::istream& instream )
: instream_( instream )
{
}

Token NewLexer::NextToken()
{
    Token ret;

    return ret;
}

