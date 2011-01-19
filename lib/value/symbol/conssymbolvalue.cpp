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
#include "lib/value/basic/pairvalue.h"
#include "lib/value/symbol/conssymbolvalue.h"
#include "lib/value/value.h"
#include "lib/argschecker.h"
#include "lib/environment.h"
#include "lib/evaluationcontext.h"
#include "lib/evaluationerror.h"
#include "lib/evaluator.h"
#include "lib/specialsymbolevaluator.h"

//virtual
const std::string& ConsSymbolValue::GetStringValue() const
{
    return StaticValue();
}

//static
const std::string& ConsSymbolValue::StaticValue()
{
    static const std::string ret( "cons" );
    return ret;
}

//virtual
ConsSymbolValue* ConsSymbolValue::Clone() const
{
    return new ConsSymbolValue( *this );
}

//virtual
SpecialSymbolEvaluator::ESymbolType ConsSymbolValue::Apply(
    EvaluationContext& ev, const CombinationValue* combo,
    std::auto_ptr<Value>& new_value, const Value*& existing_value ) const
{
    // TODO: why not a native function?

    if( combo->size() != 3 )
    {
        ArgsChecker::ThrowWrongNumArgsException( "cons", combo->size() - 1, 2 );
    }

    CombinationValue::const_iterator it = combo->begin();
    assert( it != combo->end() ); // First of 3 - "cons" (ignore)

    ++it;
    assert( it != combo->end() ); // Second of 3 - first in pair
    const Value* first = *it;

    ++it;
    assert( it != combo->end() ); // Third of 3 - second in pair

    new_value = std::auto_ptr<Value>( new PairValue(
        ev.SubEval( first ),
        ev.SubEval( *it ) ) );

    return SpecialSymbolEvaluator::eReturnNewValue;
}

