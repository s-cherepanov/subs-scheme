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
#include "lib/value/value.h"
#include "lib/environment.h"
#include "lib/evaluator.h"
#include "lib/specialsymbolevaluator.h"
#include "lib/valueutilities.h"

namespace
{

// REMOVE: copied from specialsymbolevaluator
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


/**
 * Evaluate 'and' or 'or'.  If we can tail-call optimise, we return a
 * pointer to the existing value that can be evaluated.  Otherwise,
 * set new_value_ to the newly-allocated value to return.
 */
template<class PredicateProperties>
const Value* eval_predicate( Evaluator* ev, const CombinationValue* combo,
    boost::shared_ptr<Environment>& environment,
    std::auto_ptr<Value>& new_value_, std::ostream& outstream )
{
    CombinationValue::const_iterator itlast = combo->end();
    assert( itlast != combo->begin() );
    --itlast;

    if( itlast == combo->begin() )
    {
        // There were no arguments - we must return the correct answer
        new_value_ = PredicateProperties::NoArgumentsReturnValue();
        return NULL;
    }
    
    CombinationValue::const_iterator it = combo->begin();
    assert( it != combo->end() );
    ++it;

    for( ; it != itlast; ++it )
    {
        std::auto_ptr<Value> value = ev->EvalInContext( *it, environment,
            outstream, false );
        if( PredicateProperties::EarlyExit( value.get() ) )
        {
            // One of the arguments allow us to exit early - set the answer
            new_value_ = value;
            return NULL;
        }
    }

    // None of the early arguments caused us to exit - return the last (it may
    // be either true or false).
    return *itlast;
}

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

SpecialSymbolEvaluator::ESymbolType AndSymbolValue::Apply(
    Evaluator* evaluator, const CombinationValue* combo,
    boost::shared_ptr<Environment>& environment,
    std::auto_ptr<Value>& new_value, const Value*& existing_value,
    std::ostream& outstream ) const
{
    existing_value = eval_predicate<AndProperties>( evaluator,
        combo, environment, new_value, outstream );

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

