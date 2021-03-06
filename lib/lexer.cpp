/*
 *  Subs Scheme Interpreter
 *  Copyright (C) 2010, 2011 Andy Balaam
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**/

#include <iostream>

#include "lib/lexer.h"
#include "lib/newlineprocessor.h"
#include "lib/token.h"

using namespace std;

namespace
{

Token handle_spill_char( unsigned int column, char& ret_spill_char )
{
    Token ret( ret_spill_char, Token::eTypeNormal, column - 1 );
    ret_spill_char = 0;
    return ret;
}

Token handle_string( const string& collected_string, unsigned int column )
{
    return Token( collected_string, Token::eTypeString,
        column - ( collected_string.size() + 2 ) );
}

Token handle_single_char_token( const string& collected_string,
    const char bracket, unsigned int column, char& ret_spill_char )
{
    // If bracket is the first thing we find, just return it
    if( collected_string.empty() )
    {
        return Token( bracket, Token::eTypeNormal, column - 1 );
    }
    else
    {
        // Otherwise return what we have, and remember
        // the bracket
        ret_spill_char = bracket;
        return Token( collected_string, Token::eTypeNormal,
            column - ( collected_string.size() + 1 ) );
    }
}

Token handle_newline( const string& collected_string, unsigned int& ret_column,
    bool& ret_ended_with_newline )
{
    Token ret( collected_string, Token::eTypeNormal,
        ret_column - ( collected_string.size() + 1 ) );

    // We set the ended_with_newline_ flag,
    // and our caller will call NewLine()
    // on the newline_processor (we can't
    // call it now, because the preceding
    // token hasn't been processed yet.
    ret_ended_with_newline = true;
    ret_column = 0;

    return ret;
}

Token handle_end_of_token( const string& collected_string, unsigned int column )
{
    return Token( collected_string, Token::eTypeNormal,
        column - ( collected_string.size() + 1 ) );
}

}

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
    enum EMode
    {
          eNormal
        , eComment
        , eString
    };

    string collected_string;
    unsigned int mode = eNormal;

    ended_with_newline_ = false;

    // If the last lex found a special character to end the previous token,
    // deal with it immediately.
    if( spill_char_ )
    {
        if( spill_char_ == ';' )
        {
            mode = eComment;
        }
        else if( spill_char_ == '"' )
        {
            mode = eString;
        }
        else
        {
            // Reset spill char to zero and return its value
            return handle_spill_char( column_, spill_char_ );
        }
        spill_char_ = 0;
    }

    // Otherwise, read from the stream in the normal way
    for( int i = instream_.get(); i != -1; i = instream_.get() )
    {
        char c = static_cast<char>( i );
        ++column_;

        // If we're in comment mode, skip this char unless it's newline
        if( mode == eComment )
        {
            if( c == '\n' )
            {
                // Reset the mode, and handle the newline below
                mode = eNormal;
            }
            else
            {
                continue;
            }
        }
        else if( mode == eString )
        {
            if( c == '"' )
            {
                return handle_string( collected_string, column_ );
            }
            else
            {
                collected_string += c;
                continue;
            }
        }

        switch( c )
        {
            case '(':
            case ')':
            case '\'':
            {
                return handle_single_char_token( collected_string, c, column_,
                    spill_char_ );
            }
            case '\n':
            {
                if( !collected_string.empty() )
                {
                    // If we've already got some characters, we
                    // have finished our token.
                    return handle_newline( collected_string, column_,
                        ended_with_newline_ );
                }
                else
                {
                    // Otherwise, register the newline and continue
                    column_ = 0;
                    if( newline_processor_ )
                    {
                        newline_processor_->NewLine();
                    }
                }
                break;
            }
            case ' ':
            {
                // Skip spaces at beginning
                if( !collected_string.empty() )
                {
                    // If we've already got some characters, we
                    // have finished our token.
                    return handle_end_of_token( collected_string, column_ );
                }
                break;
            }
            case '"':
            {
                if( !collected_string.empty() )
                {
                    spill_char_ = '"';
                    // If we've already got some characters, we
                    // have finished our token.
                    return handle_end_of_token( collected_string, column_ );
                }
                mode = eString;
                break;
            }
            case ';':
            {
                mode = eComment;
                break;
            }
            default:
            {
                collected_string += c;
            }
        }
    }

    // TODO: throw if we are in string mode - the string was never closed

    // We have reached the end of the stream - return the collected chars.
    // (Note this may be the empty string - this is fine if so - that
    // indicates to our caller that the stream has finished.)
    return Token( collected_string, Token::eTypeNormal,
        column_ - collected_string.size() );
}

void Lexer::SkipWhitespaceToNewline()
{
    int i = instream_.peek();

    // Loop until end of stream or something other than space or newline
    while( i != -1 && ( i == 32 || i == 10 ) )
    {
        instream_.get();
        if( i == 10 )
        {
            break;
        }
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

