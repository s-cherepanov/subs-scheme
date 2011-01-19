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

#include <string>

#include "lib/value/symbol/lambdasymbolvalue.h"
#include "lib/value/symbol/lambdautilities.h"
#include "lib/evaluationcontext.h"

//virtual
const std::string& LambdaSymbolValue::GetStringValue() const
{
    return StaticValue();
}

//static
const std::string& LambdaSymbolValue::StaticValue()
{
    static const std::string ret( "lambda" );
    return ret;
}

//virtual
LambdaSymbolValue* LambdaSymbolValue::Clone() const
{
    return new LambdaSymbolValue( *this );
}

//virtual
SpecialSymbolEvaluator::ESymbolType LambdaSymbolValue::Apply(
    EvaluationContext& ev, const CombinationValue* combo,
    std::auto_ptr<Value>& new_value, const Value*& existing_value ) const
{
    new_value = LambdaUtilities::eval_lambda( combo, ev.environment_ );

    return SpecialSymbolEvaluator::eReturnNewValue;
}

