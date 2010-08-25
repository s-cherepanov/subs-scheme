
#include <iostream>

#include "bracketmatcher.h"
#include "lexer.h"
#include "tokenlist.h"

BracketMatcher::BracketMatcher( Lexer& lexer, std::ostream& out )
: lexer_( lexer )
, newline_processor_( out )
{
    lexer_.SetNewLineProcessor( &newline_processor_ );
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

        token.column += newline_processor_.GetIndent();

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
                newline_processor_.PushIndent( bracket_level, token.column );
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
        if( token.name.empty() )
        {
            return TokenList();
        }

        ret.AddToken( token );

        // If we've finished this combination, return now
        if( token.name == ")" && bracket_level == 1 )
        {
            break;
        }

        // Otherwise, adjust our bracket level if there is a ( or )
        if( token.name == "(" )
        {
            ++bracket_level;
            mode = eOperator; // The next token will be the operator
        }
        else if( token.name == ")" )
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
        if( lexer_.EndedWithNewLine() )
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
