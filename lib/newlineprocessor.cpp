
#include <iostream>

#include "newlineprocessor.h"

using namespace std;

NewLineProcessor::NewLineProcessor( std::ostream& out )
: out_( out )
, indent_( 0 )
{
}

void NewLineProcessor::PushIndent( unsigned int level, unsigned int indent )
{
    indent_stack_.push( make_pair( level, indent ) );
}


void NewLineProcessor::PopIndent( unsigned int level )
{
    if( indent_stack_.empty() )
    {
        return;
    }

    if( indent_stack_.top().first == level )
    {
        indent_stack_.pop();
    }
}

unsigned int NewLineProcessor::GetIndent() const
{
    return indent_;
}

void NewLineProcessor::NewLine()
{
    // TODO: can we make the Repl emit this instead of doing
    //       it all the way in here?

    out_ << ". ";

    if( indent_stack_.empty() )
    {
        indent_ = 0;
    }
    else
    {
        indent_ = indent_stack_.top().second;
    }

    for( unsigned int i = 0; i < indent_; ++i )
    {
        out_ << ' ';
    }
}

void NewLineProcessor::Reset()
{
    indent_stack_ = std::stack< std::pair<unsigned int, unsigned int> >();
    indent_ = 0;
}

