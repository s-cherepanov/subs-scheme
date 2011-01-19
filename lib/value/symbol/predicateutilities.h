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

#ifndef PREDICATEUTILITIES_H
#define PREDICATEUTILITIES_H

#include <iosfwd>
#include <memory>
#include <string>

#include "lib/value/basic/combinationvalue.h"
#include "lib/value/basic/stringvalue.h"
#include "lib/value/symbol/newlinesymbolvalue.h"
#include "lib/value/value.h"
#include "lib/environment.h"
#include "lib/evaluationcontext.h"
#include "lib/evaluator.h"

namespace PredicateUtilities
{

/**
 * Evaluate 'and' or 'or'.  If we can tail-call optimise, we return a
 * pointer to the existing value that can be evaluated.  Otherwise,
 * set new_value to the newly-allocated value to return.
 */
template<class PredicateProperties>
const Value* eval_predicate( EvaluationContext& ev,
    const CombinationValue* combo, std::auto_ptr<Value>& new_value )
{
    CombinationValue::const_iterator itlast = combo->end();
    assert( itlast != combo->begin() );
    --itlast;

    if( itlast == combo->begin() )
    {
        // There were no arguments - we must return the correct answer
        new_value = PredicateProperties::NoArgumentsReturnValue();
        return NULL;
    }

    CombinationValue::const_iterator it = combo->begin();
    assert( it != combo->end() );
    ++it;

    for( ; it != itlast; ++it )
    {
        std::auto_ptr<Value> value = ev.SubEval( *it );
        if( PredicateProperties::EarlyExit( value.get() ) )
        {
            // One of the arguments allow us to exit early - set the answer
            new_value = value;
            return NULL;
        }
    }

    // None of the early arguments caused us to exit - return the last (it may
    // be either true or false).
    return *itlast;
}

}

#endif

