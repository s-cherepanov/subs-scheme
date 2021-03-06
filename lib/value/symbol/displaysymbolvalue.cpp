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

#include <iostream>
#include <memory>
#include <string>

#include "lib/value/basic/combinationvalue.h"
#include "lib/value/basic/stringvalue.h"
#include "lib/value/symbol/displaysymbolvalue.h"
#include "lib/value/value.h"
#include "lib/argschecker.h"
#include "lib/environment.h"
#include "lib/evaluationcontext.h"
#include "lib/evaluator.h"
#include "lib/prettyprinter.h"
#include "lib/specialsymbolevaluator.h"

//virtual
const std::string& DisplaySymbolValue::GetStringValue() const
{
    return StaticValue();
}

//static
const std::string& DisplaySymbolValue::StaticValue()
{
    static const std::string ret( "display" );
    return ret;
}


//virtual
DisplaySymbolValue* DisplaySymbolValue::Clone() const
{
    return new DisplaySymbolValue( *this );
}

//virtual
SpecialSymbolEvaluator::ESymbolType DisplaySymbolValue::Apply(
    EvaluationContext& ev, const CombinationValue* combo,
    std::auto_ptr<Value>& new_value, const Value*& existing_value ) const
{
    if( combo->size() != 2 )
    {
        ArgsChecker::ThrowWrongNumArgsException( "display", combo->size() - 1,
            1 );
    }

    std::auto_ptr<Value> value = ev.SubEval( (*combo)[1] );

    const StringValue* stringvalue = dynamic_cast< const StringValue* >(
        value.get() );
    if( stringvalue )
    {
        ev.GetOutStream() << stringvalue->GetStringValue();
    }
    else
    {
        ev.GetOutStream() << PrettyPrinter::Print( value.get() );
    }

    existing_value = NULL;
    return SpecialSymbolEvaluator::eEvaluateExistingSymbol;
}

