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
#include "lib/value/symbol/ifsymbolvalue.h"
#include "lib/value/value.h"
#include "lib/environment.h"
#include "lib/evaluationcontext.h"
#include "lib/evaluationerror.h"
#include "lib/evaluator.h"
#include "lib/specialsymbolevaluator.h"
#include "lib/valueutilities.h"

//virtual
const std::string& IfSymbolValue::GetStringValue() const
{
    return StaticValue();
}

//static
const std::string& IfSymbolValue::StaticValue()
{
    static const std::string ret( "if" );
    return ret;
}


//virtual
IfSymbolValue* IfSymbolValue::Clone() const
{
    return new IfSymbolValue( *this );
}

//virtual
SpecialSymbolEvaluator::ESymbolType IfSymbolValue::Apply(
    EvaluationContext& ev, const CombinationValue* combo,
    std::auto_ptr<Value>& new_value, const Value*& existing_value ) const
{
    if( combo->size() != 4 )
    {
        if( combo->size() < 4 )
        {
            // TODO: use ArgsChecker
            throw EvaluationError(
                "Not enough operands to if: there should be 3 (predicate, "
                "true-value, else-value)." );
        }
        else
        {
            // TODO: use ArgsChecker
            throw EvaluationError(
                "Too many operands to if: there should be 3 (predicate, "
                "true-value, else-value)." );
        }
    }

    CombinationValue::const_iterator it = combo->begin();
    assert( it != combo->end() ); // "if" - there are 4 items

    ++it;
    assert( it != combo->end() ); // predicate - there are 4 items
    const Value* predicate = *it;

    ++it; // Move to "true" value
    assert( it != combo->end() );

    std::auto_ptr<Value> evald_pred = ev.SubEval( predicate );

    if( ValueUtilities::IsFalse( evald_pred.get() ) )
    {
        ++it; // Move to "false" value
        assert( it != combo->end() );
    }

    existing_value = *it;

    return SpecialSymbolEvaluator::eEvaluateExistingSymbol;
}

