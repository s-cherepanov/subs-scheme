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
#include "lib/value/symbol/cdrsymbolvalue.h"
#include "lib/value/value.h"
#include "lib/argschecker.h"
#include "lib/environment.h"
#include "lib/evaluationerror.h"
#include "lib/evaluator.h"
#include "lib/prettyprinter.h"
#include "lib/specialsymbolevaluator.h"

//virtual
const std::string& CdrSymbolValue::GetStringValue() const
{
    return StaticValue();
}

//static
const std::string& CdrSymbolValue::StaticValue()
{
    static const std::string ret( "cdr" );
    return ret;
}

//virtual
CdrSymbolValue* CdrSymbolValue::Clone() const
{
    return new CdrSymbolValue( *this );
}

//virtual
SpecialSymbolEvaluator::ESymbolType CdrSymbolValue::Apply(
    Evaluator* evaluator, const CombinationValue* combo,
    boost::shared_ptr<Environment>& environment,
    std::auto_ptr<Value>& new_value, const Value*& existing_value,
    std::ostream& outstream, bool is_tail_call ) const
{
    if( combo->size() != 2 )
    {
        ArgsChecker::ThrowWrongNumArgsException( "cdr", combo->size() - 1, 1 );
    }

    CombinationValue::const_iterator it = combo->begin();
    assert( it != combo->end() ); // "cdr" - there are 2 items

    ++it;
    assert( it != combo->end() ); // the pair to extract from

    std::auto_ptr<Value> evald_arg = evaluator->EvalInContext( *it,
        environment, outstream, false );

    PairValue* pair = dynamic_cast<PairValue*>( evald_arg.get() );

    if( !pair )
    {
        throw EvaluationError( "The argument to cdr must be a pair. '" +
            PrettyPrinter::Print( evald_arg.get() ) + "' is not a pair." );
    }

    new_value = evaluator->EvalInContext( pair->GetSecond(), environment,
        outstream, false );

    return SpecialSymbolEvaluator::eReturnNewValue;
}

