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

#include <iosfwd>
#include <memory>
#include <string>

#include "lib/value/basic/combinationvalue.h"
#include "lib/value/basic/falsevalue.h"
#include "lib/value/basic/truevalue.h"
#include "lib/value/symbol/orsymbolvalue.h"
#include "lib/value/symbol/predicateutilities.h"
#include "lib/value/value.h"
#include "lib/environment.h"
#include "lib/evaluationcontext.h"
#include "lib/evaluator.h"
#include "lib/specialsymbolevaluator.h"
#include "lib/valueutilities.h"

namespace
{

class OrProperties
{
public:
    static bool EarlyExit( const Value* value )
    {
        return ValueUtilities::IsTrue( value );
    }

    static std::auto_ptr<Value> NoArgumentsReturnValue()
    {
        return std::auto_ptr<Value>( new FalseValue );
    }
};

}

//virtual
const std::string& OrSymbolValue::GetStringValue() const
{
    return StaticValue();
}

//static
const std::string& OrSymbolValue::StaticValue()
{
    static const std::string ret( "or" );
    return ret;
}

//virtual
OrSymbolValue* OrSymbolValue::Clone() const
{
    return new OrSymbolValue( *this );
}

//virtual
SpecialSymbolEvaluator::ESymbolType OrSymbolValue::Apply(
    EvaluationContext& ev, const CombinationValue* combo,
    std::auto_ptr<Value>& new_value, const Value*& existing_value ) const
{
    existing_value = PredicateUtilities::eval_predicate<OrProperties>(
        ev, combo, new_value );

    if( existing_value )
    {
        assert( !new_value.get() );
        return SpecialSymbolEvaluator::eEvaluateExistingSymbol;
    }
    else
    {
        assert( new_value.get() );
        return SpecialSymbolEvaluator::eReturnNewValue;
    }
}
