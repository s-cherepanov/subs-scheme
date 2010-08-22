
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

    int i = instream_.get();
    if( i != -1 )
    {
        ret.name += static_cast<char>( i );
    }

    return ret;
}

