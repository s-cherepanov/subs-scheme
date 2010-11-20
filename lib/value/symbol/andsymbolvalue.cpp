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

#include <iosfwd>
#include <memory>
#include <string>

#include "lib/value/basic/combinationvalue.h"
#include "lib/value/basic/falsevalue.h"
#include "lib/value/basic/truevalue.h"
#include "lib/value/symbol/andsymbolvalue.h"
#include "lib/value/symbol/predicateutilities.h"
#include "lib/value/value.h"
#include "lib/environment.h"
#include "lib/evaluator.h"
#include "lib/specialsymbolevaluator.h"
#include "lib/valueutilities.h"

namespace
{

class AndProperties
{
public:
    static bool EarlyExit( const Value* value )
    {
        return ValueUtilities::IsFalse( value );
    }

    static std::auto_ptr<Value> NoArgumentsReturnValue()
    {
        return std::auto_ptr<Value>( new TrueValue );
    }
};

}

//virtual
const std::string& AndSymbolValue::GetStringValue() const
{
    return StaticValue();
}

//static
const std::string& AndSymbolValue::StaticValue()
{
    static const std::string ret( "and" );
    return ret;
}

//virtual
AndSymbolValue* AndSymbolValue::Clone() const
{
    return new AndSymbolValue( *this );
}

//virtual
SpecialSymbolEvaluator::ESymbolType AndSymbolValue::Apply(
    Evaluator* evaluator, const CombinationValue* combo,
    boost::shared_ptr<Environment>& environment,
    std::auto_ptr<Value>& new_value, const Value*& existing_value,
    std::ostream& outstream, bool is_tail_call ) const
{
    existing_value = PredicateUtilities::eval_predicate<AndProperties>(
        evaluator, combo, environment, new_value, outstream );

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

