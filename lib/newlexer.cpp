
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
    while( i != -1 )
    {
        char c = static_cast<char>( i );
        switch( c )
        {
            case '\n':
            case ' ':
            {
                // Skip spaces at beginning
                if( !ret.name.empty() )
                {
                    // But if we're not at the beginning, we
                    // have finished our token.
                    return ret;
                }
                break;
            }
            default:
            {
                ret.name += c;
            }
        }
        i = instream_.get();
    }

    return ret;
}

