/*
 *  Subs Scheme Interpreter
 *  Copyright (C) 2010 Andy Balaam
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

#include "bracketmatcher.h"
#include "lexer.h"
#include "tokenlist.h"

BracketMatcher::BracketMatcher( Lexer& lexer, std::ostream& out,
    bool print_continuation )
: lexer_( lexer )
, newline_processor_( out )
, print_continuation_( print_continuation )
{
    if( print_continuation )
    {
        lexer_.SetNewLineProcessor( &newline_processor_ );
    }
}

BracketMatcher::~BracketMatcher()
{
    lexer_.SetNewLineProcessor( NULL );
}

TokenList BracketMatcher::ReadCombination()
{
    // TODO: split into smaller functions

    enum EMode
    {
          eOther
        , eOperator
        , eFirstOperand
    };

    TokenList ret;
    unsigned int bracket_level = 0;
    EMode mode = eOther;

    while( true )
    {
        Token token = lexer_.NextToken();

        token.AddToColumn( newline_processor_.GetIndent() );

        // If we have got to the first operand of a combination
        // store the indent level so that we can indent correctly
        // in the repl when the user presses return.
        switch( mode )
        {
            case eOperator:
            {
                mode = eFirstOperand;
                break;
            }
            case eFirstOperand:
            {
                newline_processor_.PushIndent( bracket_level, token.Column() );
                mode = eOther;
                break;
            }
            default:
            {
                break;
            }
        }

        // If the stream ended before a combo was closed, return an empty
        // list - the user pressed Ctrl-D and doesn't want any output.
        if( token.IsEndOfStream() )
        {
            return TokenList();
        }

        ret.AddToken( token );

        // If we've finished this combination, return now
        if( token.Name() == ")" && bracket_level == 1 )
        {
            break;
        }

        // Otherwise, adjust our bracket level if there is a ( or )
        if( token.Name() == "(" )
        {
            ++bracket_level;
            mode = eOperator; // The next token will be the operator
        }
        else if( token.Name() == ")" )
        {
            newline_processor_.PopIndent( bracket_level );
            --bracket_level;
            mode = eOther;
        }

        // If we got a bare symbol (no brackets) we exit here
        if( bracket_level == 0 )
        {
            break;
        }

        // If the lexer's token ended with newline, it couldn't call
        // newline (because we hadn't processed the token yet).  We
        // have now processed it, so we can call NewLine now.
        if( print_continuation_ && lexer_.EndedWithNewLine() )
        {
            newline_processor_.NewLine();
        }
    }

    // When the combination (or bare symbol) is finished, we don't
    // need to track indentation any more, so reset the newline processor,
    // and we don't care about any newlines before the next token, so
    // we skip whitespace in the lexer.
    newline_processor_.Reset();

    return ret;
}

bool BracketMatcher::eof() const
{
    return lexer_.eof();
}
