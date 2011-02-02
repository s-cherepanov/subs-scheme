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

#include "lib/newlineprocessor.h"

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

