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

#include <memory>
#include <sstream>
#include <string>

#include "lib/value/symbol/andsymbolvalue.h"
#include "lib/value/symbol/beginsymbolvalue.h"
#include "lib/value/symbol/cadrsymbolvalue.h"
#include "lib/value/symbol/condsymbolvalue.h"
#include "lib/value/symbol/conssymbolvalue.h"
#include "lib/value/symbol/customsymbolvalue.h"
#include "lib/value/symbol/definesymbolvalue.h"
#include "lib/value/symbol/displaysymbolvalue.h"
#include "lib/value/symbol/elsesymbolvalue.h"
#include "lib/value/symbol/ifsymbolvalue.h"
#include "lib/value/symbol/lambdasymbolvalue.h"
#include "lib/value/symbol/letsymbolvalue.h"
#include "lib/value/symbol/listsymbolvalue.h"
#include "lib/value/symbol/newlinesymbolvalue.h"
#include "lib/value/symbol/orsymbolvalue.h"
#include "lib/value/symbol/quotesymbolvalue.h"
#include "lib/value/basic/decimalvalue.h"
#include "lib/value/basic/integervalue.h"
#include "lib/value/basic/stringvalue.h"
#include "lib/value/value.h"
#include "lib/token.h"
#include "lib/valuefactory.h"

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

template<class T>
void add_to_map( map<string, const Value* >& special_symbols )
{
    special_symbols[ T::StaticValue() ] = new T;
}

}

ValueFactory::ValueFactory()
{
    add_to_map<AndSymbolValue>(     special_symbols_ );
    add_to_map<BeginSymbolValue>(   special_symbols_ );
    add_to_map<CondSymbolValue>(    special_symbols_ );
    add_to_map<ConsSymbolValue>(    special_symbols_ );
    add_to_map<DefineSymbolValue>(  special_symbols_ );
    add_to_map<DisplaySymbolValue>( special_symbols_ );
    add_to_map<ElseSymbolValue>(    special_symbols_ );
    add_to_map<IfSymbolValue>(      special_symbols_ );
    add_to_map<LambdaSymbolValue>(  special_symbols_ );
    add_to_map<LetSymbolValue>(     special_symbols_ );
    add_to_map<ListSymbolValue>(    special_symbols_ );
    add_to_map<NewlineSymbolValue>( special_symbols_ );
    add_to_map<OrSymbolValue>(      special_symbols_ );
    add_to_map<QuoteSymbolValue>(   special_symbols_ );
}

ValueFactory::~ValueFactory()
{
    for( map<string, const Value* >::iterator it = special_symbols_.begin();
        it != special_symbols_.end(); ++it )
    {
        delete it->second;
    }
}

std::auto_ptr<Value> ValueFactory::CreateValue( const Token& token ) const
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
            // Check for car, cdr, cadr etc.
            auto_ptr<Value> cadr = CadrSymbolValue::CreateFromString(
                token_name );
            if( cadr.get() )
            {
                return cadr;
            }

            // Otherwise, look up in list of fixed symbols
            // TODO: case insensitive
            map< string, const Value* >::const_iterator itFind =
                special_symbols_.find( token_name );

            if( itFind != special_symbols_.end() )
            {
                return auto_ptr<Value>( itFind->second->Clone() );
            }
            else
            {
                return auto_ptr<Value>( new CustomSymbolValue( token_name ) );
            }
        }
    }
}

