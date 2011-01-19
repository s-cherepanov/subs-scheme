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
#include "lib/value/symbol/definesymbolvalue.h"
#include "lib/value/symbol/defineutilities.h"
#include "lib/value/value.h"
#include "lib/argschecker.h"
#include "lib/environment.h"
#include "lib/evaluationcontext.h"
#include "lib/evaluationerror.h"
#include "lib/evaluator.h"
#include "lib/prettyprinter.h"
#include "lib/specialsymbolevaluator.h"

using namespace std;

namespace
{

std::auto_ptr<Value> eval_define_symbol( Evaluator* ev,
    boost::shared_ptr<Environment>& environment,
    const SymbolValue* to_define, const Value* definition,
    std::ostream& outstream )
{
    auto_ptr<Value> value = ev->EvalInContext( definition, environment,
        outstream, false );

    DefineUtilities::insert_value_into_environment( environment,
        to_define->GetStringValue(), value );

    return auto_ptr<Value>( to_define->Clone() );
}

std::auto_ptr<Value> eval_define( Evaluator* evaluator,
    const CombinationValue* combo, boost::shared_ptr<Environment>& environment,
    std::ostream& outstream )
{
    if( combo->size() < 3 )
    {
        throw EvaluationError(
            "Too few operands to the define operator: there should be 2." );
    }

    CombinationValue::const_iterator it = combo->begin();
    assert( it != combo->end() );
    ++it;
    assert( it != combo->end() );

    const Value* to_define = *it;

    ++it;
    assert( it != combo->end() );

    const CombinationValue* comb_to_define =
        dynamic_cast<const CombinationValue*>( to_define );

    if( comb_to_define )
    {
        CombinationValue::const_iterator itarg = comb_to_define->begin();

        if( itarg == comb_to_define->end() )
        {
            throw EvaluationError(
                "The first operand to define must be a symbol or a non-empty "
                "combination.  The combination supplied is empty." );
        }

        const SymbolValue* sym = dynamic_cast<const SymbolValue*>( *itarg );
        if( !sym )
        {
            throw EvaluationError( "The first element in the list defining "
                "a procedure must be a symbol - '" + PrettyPrinter::Print(
                    to_define ) + "' is not a symbol." );
        }

        ++itarg; // Move on from the procedure name to the arguments

        return eval_define_symbol( evaluator, environment, sym,
            DefineUtilities::define_procedure( itarg, comb_to_define->end(), it,
                combo->end(), environment, sym->GetStringValue() ).get(),
                outstream );
    }
    else
    {
        if( combo->size() > 3 )
        {
            ArgsChecker::ThrowWrongNumArgsException( "define",
                combo->size() - 1, 2 );
        }

        const SymbolValue* sym = dynamic_cast<const SymbolValue*>( to_define );
        if( !sym )
        {
            throw EvaluationError( "The first operand to the define operator "
                "must be a symbol or a combination. '" + PrettyPrinter::Print(
                    to_define ) + "' is neither." );
        }

        return eval_define_symbol( evaluator, environment, sym, *it,
            outstream );
    }
}


}

//virtual
const std::string& DefineSymbolValue::GetStringValue() const
{
    return StaticValue();
}

//static
const std::string& DefineSymbolValue::StaticValue()
{
    static const std::string ret( "define" );
    return ret;
}

//virtual
DefineSymbolValue* DefineSymbolValue::Clone() const
{
    return new DefineSymbolValue( *this );
}

//virtual
SpecialSymbolEvaluator::ESymbolType DefineSymbolValue::Apply(
    EvaluationContext& ev, const CombinationValue* combo,
    std::auto_ptr<Value>& new_value, const Value*& existing_value ) const
{
    new_value = eval_define( ev.evaluator_, combo, ev.environment_,
        ev.outstream_ );

    return SpecialSymbolEvaluator::eReturnNewValue;
}


