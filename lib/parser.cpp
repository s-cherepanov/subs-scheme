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

#include "combinationvalue.h"
#include "ilexer.h"
#include "parsingerror.h"
#include "parser.h"
#include "token.h"
#include "unfinishedcombinationexception.h"
#include "valuefactory.h"

using namespace std;

namespace
{

std::auto_ptr<Value> next_value_from_token( ILexer& lexer,
    Token token )
{
    if( token.IsEndOfStream() )
    {
        return std::auto_ptr<Value>( NULL );
    }

    if( token.Name() == ")" )
    {
        throw ParsingError(); // TODO: line number etc.
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

            ret->push_back( next_value_from_token( lexer, token ).release() );
        }
        return auto_ptr<Value>( ret.release() );
    }
    else
    {
        return ValueFactory::CreateValue( token );
    }
}


}

Parser::Parser( ILexer& lexer )
: lexer_( lexer )
{
}

std::auto_ptr<Value> Parser::NextValue()
{
    return next_value_from_token( lexer_, lexer_.NextToken() );
}

