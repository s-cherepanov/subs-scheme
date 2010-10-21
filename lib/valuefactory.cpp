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
#include <sstream>
#include <string>

#include "value/symbol/customsymbolvalue.h"
#include "value/symbol/specialsymbolvalue.h"

#include "decimalvalue.h"
#include "integervalue.h"
#include "stringvalue.h"
#include "token.h"
#include "value.h"
#include "valuefactory.h"

using namespace std;

namespace
{

enum ENumericType
{
      nonnumeric
    , integer
    , decimal
};

ENumericType get_numeric_type( const string& str )
{
    bool seen_dot = false;
    bool this_is_dot = false;

    string::const_iterator it = str.begin();

    // Minus sign is valid at the beginning
    if( *it == '-' )
    {
        ++it;
        // but not if it's on its own
        if( it == str.end() )
        {
            return nonnumeric;
        }
    }

    for( ; it != str.end(); ++it )
    {
        this_is_dot = false;

        char ch = *it;

        if( ch == '.' )
        {
            if( seen_dot )
            {
                return nonnumeric;
            }
            seen_dot = true;
            this_is_dot = true;
        }
        else if( ch < '0' or ch > '9' )
        {
            return nonnumeric;
        }
    }

    if( this_is_dot )
    {
        return nonnumeric;
    }
    else if( seen_dot )
    {
        return decimal;
    }
    else
    {
        return integer;
    }
}

}

namespace ValueFactory
{

std::auto_ptr<Value> CreateValue( const Token& token )
{
    const string& token_name = token.Name();

    if( token.Type() == Token::eTypeString )
    {
        return auto_ptr<Value>( new StringValue( token_name ) );
    }

    switch( get_numeric_type( token_name ) )
    {
        case integer:
        {
            // TODO: handle large integers
            istringstream ss( token_name );
            int intvalue;
            ss >> intvalue;
            return auto_ptr<Value>( new IntegerValue( intvalue ) );
        }
        case decimal:
        {
            // TODO: handle large decimals
            istringstream ss( token_name );
            double doublevalue;
            ss >> doublevalue;
            return auto_ptr<Value>( new DecimalValue( doublevalue ) );
        }
        default:
        {
            SpecialSymbolValue::SymbolType symtype =
                SpecialSymbolValue::String2SymbolType( token_name );

            if( symtype == SpecialSymbolValue::t_CUSTOM )
            {
                return auto_ptr<Value>( new CustomSymbolValue( token_name ) );
            }
            else
            {
                return auto_ptr<Value>( new SpecialSymbolValue( symtype ) );
            }
        }
    }
}

};

