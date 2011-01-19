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
#include "lib/value/symbol/beginsymbolvalue.h"
#include "lib/value/value.h"
#include "lib/environment.h"
#include "lib/evaluationcontext.h"
#include "lib/evaluator.h"
#include "lib/specialsymbolevaluator.h"


//virtual
const std::string& BeginSymbolValue::GetStringValue() const
{
    return StaticValue();
}

//static
const std::string& BeginSymbolValue::StaticValue()
{
    static const std::string ret( "begin" );
    return ret;
}

//virtual
BeginSymbolValue* BeginSymbolValue::Clone() const
{
    return new BeginSymbolValue( *this );
}

//virtual
SpecialSymbolEvaluator::ESymbolType BeginSymbolValue::Apply(
    EvaluationContext& ev, const CombinationValue* combo,
    std::auto_ptr<Value>& new_value, const Value*& existing_value ) const
{
    CombinationValue::const_iterator it = combo->begin();
    CombinationValue::const_iterator itend = combo->end();

    assert( it != itend ); // The combo must contain the begin symbol itself

    ++it; // Skip "begin"
    if( it == itend )
    {
        // Called with no arguments - return NULL
        existing_value = NULL;
        return SpecialSymbolEvaluator::eEvaluateExistingSymbol;
    }

    --itend; // Step back one: the last argument is handled differently
             // (to allow tail-call recursion to work).

    for( ; it != itend; ++it )
    {
        ev.SubEval( *it );
    }

    existing_value = *it;
    return SpecialSymbolEvaluator::eEvaluateExistingSymbol;
}

