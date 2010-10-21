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

#include <vector>

#include "lib/token.h"
#include "lib/tokenlist.h"

void TokenList::AddToken( const Token& token )
{
    list_.push( token );
}

//virtual
Token TokenList::NextToken()
{
    if( list_.empty() )
    {
        // Return a token indicating end of stream
        return Token( "", Token::eTypeNormal, 0 );
    }

    Token ret = list_.front();
    list_.pop();
    return ret;
}


bool TokenList::empty() const
{
    return list_.empty();
}

