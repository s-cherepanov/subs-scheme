
#include <iostream>

#include "lexer.h"
#include "newlineprocessor.h"
#include "token.h"


Lexer::Lexer( std::istream& instream )
: instream_( instream )
, spill_char_( 0 )
, column_( 0 )
, newline_processor_( NULL )
{
}

//virtual
Token Lexer::NextToken()
{
    Token ret;

    ended_with_newline_ = false;

    // If the last lex found a character we need to return this time,
    // return it straight away.
    if( spill_char_ )
    {
        ret.name += spill_char_;
        ret.column = column_ - 1;
        spill_char_ = 0;
        return ret;
    }

    // Otherwise, read from the stream in the normal way
    int i = instream_.get();
    while( i != -1 )
    {
        char c = static_cast<char>( i );
        ++column_;
        switch( c )
        {
            case '(':
            case ')':
            {
                // If bracket is the first thing we find, just return it
                if( ret.name.empty() )
                {
                    ret.name += c;
                    ret.column = column_ - 1;
                    return ret;
                }
                else
                {
                    // Otherwise return what we have, and remember
                    // the bracket
                    spill_char_ = c;
                    ret.column = column_ - ( ret.name.size() + 1 );
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
                    ret.column = column_ - ( ret.name.size() + 1 );
                    if( c == '\n' )
                    {
                        // We set the ended_with_newline_ flag,
                        // and our caller will call NewLine()
                        // on the newline_processor (we can't
                        // call it now, because the preceding
                        // token hasn't been processed yet.
                        ended_with_newline_ = true;
                        column_ = 0;
                    }
                    return ret;
                }
                else if( c == '\n' )
                {
                    column_ = 0;
                    if( newline_processor_ )
                    {
                        newline_processor_->NewLine();
                    }
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

    ret.column = column_ - ret.name.size();
    return ret;
}

void Lexer::SkipWhitespace()
{
    int i = instream_.peek();

    // Loop until end of stream or something other than space or newline
    while( i != -1 && ( i == 32 || i == 10 ) )
    {
        instream_.get();
        i = instream_.peek();
    }
}


void Lexer::SetNewLineProcessor( NewLineProcessor* newline_processor )
{
    newline_processor_ = newline_processor;
}

bool Lexer::EndedWithNewLine() const
{
    return ended_with_newline_;
}

bool Lexer::eof() const
{
    return instream_.eof();
}

