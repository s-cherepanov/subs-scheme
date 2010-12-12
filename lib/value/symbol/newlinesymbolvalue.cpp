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

#include <iostream>
#include <memory>
#include <string>

#include "lib/value/basic/combinationvalue.h"
#include "lib/value/basic/stringvalue.h"
#include "lib/value/symbol/newlinesymbolvalue.h"
#include "lib/value/value.h"
#include "lib/argschecker.h"
#include "lib/environment.h"
#include "lib/evaluator.h"
#include "lib/specialsymbolevaluator.h"

//virtual
const std::string& NewlineSymbolValue::GetStringValue() const
{
    return StaticValue();
}

//static
const std::string& NewlineSymbolValue::StaticValue()
{
    static const std::string ret( "newline" );
    return ret;
}

//virtual
NewlineSymbolValue* NewlineSymbolValue::Clone() const
{
    return new NewlineSymbolValue( *this );
}

//virtual
SpecialSymbolEvaluator::ESymbolType NewlineSymbolValue::Apply(
    Evaluator* evaluator, const CombinationValue* combo,
    boost::shared_ptr<Environment>& environment,
    std::auto_ptr<Value>& new_value, const Value*& existing_value,
    std::ostream& outstream, bool is_tail_call ) const
{
    if( combo->size() > 1 )
    {
        ArgsChecker::ThrowWrongNumArgsException( "newline", combo->size() - 1,
            0 );
    }

    outstream << std::endl;

    existing_value = NULL;
    return SpecialSymbolEvaluator::eEvaluateExistingSymbol;
}
