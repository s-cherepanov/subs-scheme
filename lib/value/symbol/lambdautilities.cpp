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
#include <boost/shared_ptr.hpp>

#include "lib/value/basic/combinationvalue.h"
#include "lib/value/symbol/defineutilities.h"
#include "lib/value/symbol/lambdautilities.h"
#include "lib/value/value.h"
#include "lib/environment.h"
#include "lib/evaluationerror.h"
#include "lib/evaluator.h"
#include "lib/prettyprinter.h"

namespace LambdaUtilities
{

std::auto_ptr<Value> eval_lambda( const CombinationValue* combo,
    const boost::shared_ptr<Environment>& environment )
{
    if( combo->size() < 3 )
    {
        throw EvaluationError(
            "Too few operands to the lambda operator: there should "
            "be  at least 2." );
    }

    CombinationValue::const_iterator it = combo->begin();
    assert( it != combo->end() ); // First of at least 3 - "lambda" (ignore)
    ++it;
    assert( it != combo->end() ); // Second of at least 3 - arguments

    const CombinationValue* args = dynamic_cast<const CombinationValue*>( *it );
    if( !args )
    {
        throw EvaluationError( "The first operand to the lambda operator "
            "must be a combination naming its arguments.  '"
            + PrettyPrinter::Print( *it )
            + "' is not a combination." );
    }

    ++it;
    assert( it != combo->end() ); // Third of at least 3 - procedure body

    // Copy the whole arguments combo as the args of the procedure, and
    // copy the rest of the main combo (unevaluated) as the body
    // of the function
    return DefineUtilities::define_procedure( args->begin(), args->end(), it,
        combo->end(), environment );
}

}

