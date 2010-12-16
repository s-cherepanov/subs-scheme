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

#include <memory>

#include "lib/value/basic/combinationvalue.h"
#include "lib/ilexer.h"
#include "lib/parser.h"
#include "lib/token.h"
#include "lib/unfinishedcombinationexception.h"
#include "lib/unmatchedclosebracketexception.h"
#include "lib/valuefactory.h"

using namespace std;

namespace
{

std::auto_ptr<Value> next_value_from_token( ILexer& lexer,
    Token token, const ValueFactory& value_factory )
{
    if( token.IsEndOfStream() )
    {
        return std::auto_ptr<Value>( NULL );
    }

    if( token.Name() == ")" )
    {
        throw UnmatchedCloseBracketException(); // TODO: line number etc.
    }

    if( token.Name() == "(" )
    {
        auto_ptr<CombinationValue> ret( new CombinationValue );

        while( true )
        {
            token = lexer.NextToken();

            if( token.IsEndOfStream() )
            {
                throw UnfinishedCombinationException();
            }
            else if( token.Name() == ")" )
            {
                break;
            }

            ret->push_back( next_value_from_token( lexer, token,
                value_factory ).release() );
        }
        return auto_ptr<Value>( ret.release() );
    }
    else
    {
        return value_factory.CreateValue( token );
    }
}


}

Parser::Parser( ILexer& lexer )
: lexer_( lexer )
{
}

std::auto_ptr<Value> Parser::NextValue()
{
    return next_value_from_token( lexer_, lexer_.NextToken(), value_factory_ );
}

