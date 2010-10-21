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

#include <string>

#include "lib/token.h"

Token::Token( char name, Token::EType type, unsigned int column )
: name_( 1, name )
, type_( type )
, column_( column )
{
}

/**
 * Create a token.  If name is the empty string, this token is the
 * end of the stream.
 */
Token::Token( const std::string& name, Token::EType type, unsigned int column )
: name_( name )
, type_( type )
, column_( column )
{
}

const std::string& Token::Name() const
{
    return name_;
}

Token::EType Token::Type() const
{
    return type_;
}

unsigned int Token::Column() const
{
    return column_;
}

bool Token::IsEndOfStream() const
{
    return ( type_ == Token::eTypeNormal ) && name_.empty();
}

void Token::AddToColumn( unsigned int adjustment )
{
    column_ += adjustment;
}

