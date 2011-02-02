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

#include "lib/value/basic/combinationvalue.h"
#include "lib/value/basic/nilvalue.h"
#include "lib/value/basic/pairvalue.h"
#include "lib/value/symbol/quotesymbolvalue.h"
#include "lib/value/value.h"
#include "lib/argschecker.h"

namespace
{

std::auto_ptr<Value> unquote( const Value* quotedvalue );

std::auto_ptr<Value> combo_to_list( CombinationValue::const_iterator it,
    const CombinationValue* combo )
{
    // TODO: make combo and list the same thing, so here we are just
    //       flipping a boolean flag?

    if( it == combo->end() )
    {
        return std::auto_ptr<Value>( new NilValue );
    }

    const Value* value = *it;

    ++it;

    return std::auto_ptr<Value>( new PairValue(
        std::auto_ptr<Value>( unquote( value ) ),
        combo_to_list( it, combo ) ) );
}

/**
 * Convert this and anything in it, turning each combination
 * into a list.
 */
std::auto_ptr<Value> unquote( const Value* quotedvalue )
{
    // If the quoted thing is a combination, "unquote" it by converting
    // it to a list of symbols.

    const CombinationValue* quotedcombo =
        dynamic_cast<const CombinationValue*>( quotedvalue );

    if( quotedcombo )
    {
        return combo_to_list( quotedcombo->begin(), quotedcombo );
    }
    else
    {
        return std::auto_ptr<Value>( quotedvalue->Clone() );
    }

}

}

//virtual
const std::string& QuoteSymbolValue::GetStringValue() const
{
    return StaticValue();
}

//static
const std::string& QuoteSymbolValue::StaticValue()
{
    static const std::string ret( "quote" );
    return ret;
}

//virtual
QuoteSymbolValue* QuoteSymbolValue::Clone() const
{
    return new QuoteSymbolValue( *this );
}

//virtual
SpecialSymbolEvaluator::ESymbolType QuoteSymbolValue::Apply(
    EvaluationContext& ev, const CombinationValue* combo,
    std::auto_ptr<Value>& new_value, const Value*& existing_value ) const
{
    if( combo->size() != 2 )
    {
        ArgsChecker::ThrowWrongNumArgsException( "quote", combo->size() - 1,
            1 );
    }

    new_value = unquote( (*combo)[1] );

    return SpecialSymbolEvaluator::eReturnNewValue;
}


