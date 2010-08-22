
#include <iostream>

#include "lexer.h"
#include "token.h"

Lexer::Lexer( std::istream& instream )
: instream_( instream )
, spill_char_( 0 )
{
}

Token Lexer::NextToken()
{
    Token ret;

    // If the last lex found a character we need to return this time,
    // return it straight away.
    if( spill_char_ )
    {
        ret.name += spill_char_;
        spill_char_ = 0;
        return ret;
    }

    // Otherwise, read from the stream in the normal way
    int i = instream_.get();
    while( i != -1 )
    {
        char c = static_cast<char>( i );
        switch( c )
        {
            case '(':
            case ')':
            {
                // If bracket is the first thing we find, just return it
                if( ret.name.empty() )
                {
                    ret.name += c;
                    return ret;
                }
                else
                {
                    // Otherwise return what we have, and remember
                    // the bracket
                    spill_char_ = c;
                    return ret;
                }
                break;
            }
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

