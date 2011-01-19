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
#include "lib/value/basic/nilvalue.h"
#include "lib/value/basic/pairvalue.h"
#include "lib/value/symbol/listsymbolvalue.h"
#include "lib/value/value.h"
#include "lib/environment.h"
#include "lib/evaluationcontext.h"
#include "lib/evaluator.h"
#include "lib/specialsymbolevaluator.h"

namespace
{

std::auto_ptr<Value> eval_list_elems( CombinationValue::const_iterator it,
    EvaluationContext& ev, const CombinationValue* combo )
{
    if( it == combo->end() )
    {
        return std::auto_ptr<Value>( new NilValue );
    }

    const Value* value = *it;

    ++it;

    return std::auto_ptr<Value>( new PairValue(
        ev.evaluator_->EvalInContext( value, ev.environment_, ev.outstream_,
            false ), eval_list_elems( it, ev, combo ) ) );
}

}

//virtual
const std::string& ListSymbolValue::GetStringValue() const
{
    return StaticValue();
}

//static
const std::string& ListSymbolValue::StaticValue()
{
    static const std::string ret( "list" );
    return ret;
}

//virtual
ListSymbolValue* ListSymbolValue::Clone() const
{
    return new ListSymbolValue( *this );
}

//virtual
SpecialSymbolEvaluator::ESymbolType ListSymbolValue::Apply(
    EvaluationContext& ev, const CombinationValue* combo,
    std::auto_ptr<Value>& new_value, const Value*& existing_value ) const
{
    // TODO: Consider implementing a ListValue that allocates no Pair objects.
    //       This could also significantly improve performance of map & filter

    CombinationValue::const_iterator it = combo->begin();

    assert( it != combo->end() ); // "list" (ignore)

    ++it;

    new_value = eval_list_elems( it, ev, combo );

    return SpecialSymbolEvaluator::eReturnNewValue;
}


