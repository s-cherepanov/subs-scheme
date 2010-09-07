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

#include <cassert>
#include <sstream>
#include <typeinfo>

#include "builtins.h"
#include "combinationvalue.h"
#include "environment.h"
#include "environmentholder.h"
#include "evaluationerror.h"
#include "equalsnativefunctionvalue.h"
#include "nativefunctionvalue.h"
#include "prettyprinter.h"
#include "specialsymbolevaluator.h"
#include "symbolvalue.h"
#include "tracer.h"
#include "compoundprocedurevalue.h"
#include "value.h"

#include "evaluator.h"

using namespace std;

namespace
{

std::auto_ptr<Value> eval_symbol( const SymbolValue* sym,
    Environment& environment )
{
    const Value* value = environment.FindSymbol( sym->GetStringValue() );

    if( !value )
    {
        throw EvaluationError( "Undefined symbol '"
            + PrettyPrinter::Print( sym ) + "'." );
    }

    // TODO: avoid the copy here?
    return auto_ptr<Value>( value->Clone() );
}



class EvalDepthMarker
{
public:
    EvalDepthMarker( Tracer* tracer )
    : tracer_( tracer )
    {
        tracer_->IncreaseEvalDepth();
    }

    ~EvalDepthMarker()
    {
       tracer_->DecreaseEvalDepth();
    }

private:
    Tracer* tracer_;
};


}

Evaluator::Evaluator()
{
    tracer_ = &null_tracer_;
    BuiltIns::Init( global_environment_ );
}

std::auto_ptr<Value> Evaluator::Eval( const Value* value,
    std::ostream& outstream )
{
    // When the user entered the empty string, we have a NULL value.
    // In this case, we simply return NULL here.
    if( value )
    {
        return EvalInContext( value, global_environment_, outstream );
    }
    else
    {
        return auto_ptr<Value>( NULL );
    }
}


std::auto_ptr<Value> Evaluator::EvalInContext( const Value* value,
    Environment& environment, std::ostream& outstream )
{
    EvalDepthMarker dm( GetTracer() );

    // TODO: replace switch-style dispatch with a virtual method on Value

    auto_ptr<Value> new_value;
    EnvironmentHolder env_holder;
    Environment* env_ptr = &environment;

    // This loop continues every time we can do a tail-call optimisation.
    while( true )
    {

        // If value is a symbol, we look it up and return the result
        const SymbolValue* plainsym = dynamic_cast<const SymbolValue*>(
            value );
        if( plainsym )
        {
            return eval_symbol( plainsym, *env_ptr );
        }

        // Maybe value is a combination?
        const CombinationValue* combo = dynamic_cast<const CombinationValue*>(
            value );

        // If it's not a combination it must be a simple value like
        // an Integer or a Procedure.  Return a copy of it.
        if( !combo )
        {
            return auto_ptr<Value>( value->Clone() );
        }

        // Now we deal with a combination.

        // First check it's non-empty
        if( combo->empty() )
        {
            // TODO: supply line number, file etc.
            throw EvaluationError(
                "Attepted to evaluate an empty combination" );
        }

        SpecialSymbolEvaluator special_symbol_evaluator( this, outstream );

        switch( special_symbol_evaluator.ProcessSpecialSymbol( combo,
            *env_ptr ) )
        {
            case SpecialSymbolEvaluator::eReturnNewValue:
            {
                // This special symbol gave us a result - return it
                // (e.g. define, lambda)
                return special_symbol_evaluator.NewValue();
            }
            case SpecialSymbolEvaluator::eEvaluateExistingSymbol:
            {
                // This was an if or cond or similar, and we can
                // go straight on to evaluate a sub-clause (allowing
                // tail-call optimisation), change value, and jump
                // back to the next time around the loop.
                value = special_symbol_evaluator.ExistingValue();
                if( value )
                {
                    continue;
                }
                else
                {
                    // Some expressions result in NULL, e.g. a cond where
                    // no condition matched.  If so, return NULL directly.
                    return auto_ptr<Value>( NULL );
                }
            }
            default:
            {
                // Otherwise this was not a special symbol, so continue
                // to dealing with a compouund procedure.
                break;
            }
        }

        CombinationValue::const_iterator cmbit = combo->begin();

        // Otherwise we evaluate the operator
        auto_ptr<Value> evaldoptr = EvalInContext( *cmbit, *env_ptr,
            outstream );

        ++cmbit; // Skip past the procedure to the arguments

        // Evaluate the arguments into a new combo
        auto_ptr<CombinationValue> argvalues( new CombinationValue );
        for( ; cmbit != combo->end(); ++cmbit )
        {
            argvalues->push_back( EvalInContext( *cmbit, *env_ptr, outstream
                ).release() );
        }

        // If it's a built-in procedure, simply run it
        const NativeFunctionValue* bip = dynamic_cast<const NativeFunctionValue*>(
            evaldoptr.get() );

        if( bip )
        {
            return bip->Run( argvalues.get() );
        }

        // Otherwise, it must be a compound procedure
        const CompoundProcedureValue* proc = dynamic_cast<
            const CompoundProcedureValue*>( evaldoptr.get() );

        if( !proc )
        {
            // TODO: shouldn't need pretty printer in places like here
            throw EvaluationError( "Attempted to run '"
                + PrettyPrinter::Print( evaldoptr.get() )
                + "', which is not a procedure." );
        }

        // Replace the current environment with one containing the
        // original environment and the argument values of the new
        // procedure.
        env_holder.push_back( proc->ExtendEnvironmentWithArgs( argvalues.get(),
            *env_ptr ).release() );
        env_ptr = env_holder.back();

        CombinationValue::const_iterator itbody = proc->GetBody()->begin();
        CombinationValue::const_iterator itbodyend = proc->GetBody()->end();
        assert( itbody != itbodyend ); // Since this procedure must have a body

        // Step back one - the last section of the body is handled differently
        --itbodyend;

        // Evaluate all elements except the last one
        for( ; itbody != itbodyend; ++itbody )
        {
            // eval_in_context returns an auto_ptr, so
            // each returned value will be deleted.
            EvalInContext( *itbody, *env_ptr, outstream );
        }

        // Now we have the last bit of the body, which is the bit
        // we will tail-call optimise by setting value to it, and
        // going around the loop again.
        new_value.reset( (*itbody)->Clone() );
        value = new_value.get();
    }

}


void Evaluator::SetTracer( Tracer* tracer )
{
    tracer_ = tracer;
}

Tracer* Evaluator::GetTracer()
{
    return tracer_;
}

