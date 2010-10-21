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

#include <cassert>
#include <string>
#include <vector>

#include "lib/value/symbol/specialsymbolvalue.h"

using namespace std;

namespace
{

const string* get_symbols( const string*& ret_end )
{
    // TODO: case insensitive
    // NOTE: keep in sync with enum SymbolType in header (including length)
    static const string ret[] = {
          "and"
        , "car"
        , "cdr"
        , "cond"
        , "cons"
        , "define"
        , "display"
        , "else"
        , "if"
        , "lambda"
        , "let"
        , "list"
        , "newline"
        , "or"
    };

    ret_end = ret + ( sizeof( ret ) / sizeof( string ) );

    return ret;
}

}


SpecialSymbolValue::SpecialSymbolValue( SymbolType symbol_type )
: symbol_type_( symbol_type )
{
    assert( symbol_type >= 0 ); // This is not a custom symbol
}

//virtual
const std::string& SpecialSymbolValue::GetStringValue() const
{
    const string* unused;
    return get_symbols( unused )[ static_cast<int>( symbol_type_ ) ];
}

//virtual
SpecialSymbolValue* SpecialSymbolValue::Clone() const
{
    return new SpecialSymbolValue( *this );
}

SpecialSymbolValue::SymbolType SpecialSymbolValue::GetSymbolType() const
{
    return symbol_type_;
}

//static
SpecialSymbolValue::SymbolType SpecialSymbolValue::String2SymbolType(
    const std::string& symbol_name )
{
    const string* symbols_end;
    const string* symbols = get_symbols( symbols_end );

    for( const string* i = symbols; i != symbols_end; ++i )
    {
        if( *i == symbol_name )
        {
            return static_cast<SymbolType>( i - symbols );
        }
    }

    return t_CUSTOM;
}

