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
#include "evaluationerror.h"
#include "equalsnativefunctionvalue.h"
#include "falsevalue.h"
#include "nativefunctionvalue.h"
#include "prettyprinter.h"
#include "symbolvalue.h"
#include "tracer.h"
#include "compoundprocedurevalue.h"
#include "value.h"

#include "evaluator.h"

using namespace std;

namespace
{

std::auto_ptr<Value> eval_in_context( Evaluator* ev, const Value* value,
    Environment& environment );
    
bool is_define_symbol( const SymbolValue& sym )
{
    // TODO: case insensitive?
    return ( sym.GetStringValue() == "define" );
}


bool is_if_symbol( const SymbolValue& sym )
{
    // TODO: case insensitive?
    return ( sym.GetStringValue() == "if" );
}

bool is_cond_symbol( const SymbolValue& sym )
{
    // TODO: case insensitive?
    return ( sym.GetStringValue() == "cond" );
}


bool is_lambda_symbol( const SymbolValue& sym )
{
    // TODO: case insensitive?
    return ( sym.GetStringValue() == "lambda" );
}


std::auto_ptr<Value> eval_define_symbol( Evaluator* ev,
    Environment& environment, const SymbolValue* to_define,
    const Value* definition )
{
    environment.InsertSymbol( to_define->GetStringValue(),
        eval_in_context( ev, definition, environment ).release() );

    return auto_ptr<Value>( to_define->Clone() );
}




/**
 * Starting at it, clone Values into a new CombinationValue until we hit end.
 *
 * If the value of it supplied is the same as "begin()", this is identical
 * to calling "Clone()" on the combination we are iterating through.
 */
auto_ptr<CombinationValue> clone_partial_combo(
    CombinationValue::const_iterator it,
    const CombinationValue::const_iterator& end )
{
    auto_ptr<CombinationValue> ret( new CombinationValue );
    for( ; it != end; ++it )
    {
        ret->push_back( (*it)->Clone() );
    }
    return ret;
}

std::auto_ptr<Value> define_procedure(
    CombinationValue::const_iterator itarg,
    const CombinationValue::const_iterator& argsend,
    CombinationValue::const_iterator itbody,
    const CombinationValue::const_iterator& bodyend,
    const std::string& name = "" )
{
    return auto_ptr<Value>( new CompoundProcedureValue(
        clone_partial_combo( itarg, argsend ).release(),
        clone_partial_combo( itbody, bodyend ).release(), name ) );
}



std::auto_ptr<Value> eval_define( Evaluator* ev, const CombinationValue* combo,
    Environment& environment )
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

        return eval_define_symbol( ev, environment, sym,
            define_procedure( itarg, comb_to_define->end(), it,
                combo->end(), sym->GetStringValue() ).get() );
    }
    else
    {
        if( combo->size() > 3 )
        {
            throw EvaluationError(
                "Too many operands to the define operator: there should "
                "be 2." );
        }

        const SymbolValue* sym = dynamic_cast<const SymbolValue*>( to_define );
        if( !sym )
        {
            throw EvaluationError( "The first operand to the define operator "
                "must be a symbol or a combination. '" + PrettyPrinter::Print(
                    to_define ) + "' is neither." );
        }

        return eval_define_symbol( ev, environment, sym, *it );
    }
}



std::auto_ptr<Value> eval_lambda( const CombinationValue* combo )
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
    return define_procedure( args->begin(), args->end(), it, combo->end() );
}


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


bool is_false( const Value* value )
{
    return dynamic_cast<const FalseValue*>( value );
}

bool is_true( const Value* value )
{
    return !is_false( value );
}


const Value* process_if( Evaluator* ev, const CombinationValue* combo,
    Environment& environment )
{
    // TODO: only one if needed here (in the normal case)

    if( combo->size() < 4 )
    {
        throw EvaluationError(
            "Not enough operands to if: there should be 3 (predicate, "
            "true-value, else-value)." );
    }

    if( combo->size() > 4 )
    {
        throw EvaluationError(
            "Too many operands to if: there should be 3 (predicate, "
            "true-value, else-value)." );
    }

    CombinationValue::const_iterator it = combo->begin();
    assert( it != combo->end() ); // "if" - there are 4 items

    ++it;
    assert( it != combo->end() ); // predicate - there are 4 items
    const Value* predicate = *it;

    ++it; // Move to "true" value
    assert( it != combo->end() );

    std::auto_ptr<Value> evald_pred = eval_in_context( ev, predicate,
        environment );

    if( is_false( evald_pred.get() ) )
    {
        ++it; // Move to "false" value
        assert( it != combo->end() );
    }

    return *it;
}



const Value* get_test_from_testvalue_pair( const Value* pair )
{
    const CombinationValue* paircombo = dynamic_cast<const CombinationValue*>(
        pair );

    if( !paircombo )
    {
        throw EvaluationError( "The operands to 'cond' must be pairs.  '"
            + PrettyPrinter::Print( pair )
            + "' is not a combination." );
    }

    if( paircombo->size() != 2 )
    {
        ostringstream ss;
        ss  << "The operands to 'cond' must be pairs.  '"
            << PrettyPrinter::Print( pair )
            << "' has " << paircombo->size()
            << " elements, where it should have 2.";

        throw EvaluationError( ss.str() );
    }

    return *paircombo->begin();
}

const Value* get_value_from_testvalue_pair( const Value* pair )
{
    // TODO: avoid doing the same cast here and in get_test_from_testvalue_pair
    const CombinationValue* paircombo = dynamic_cast<const CombinationValue*>(
        pair );

    assert( paircombo ); // We shouldn't get here without checking this in
                         // get_test_from_testvalue_pair
    assert( paircombo->size() == 2 ); // We shouldn't get here without
                                      // checking this in
                                      // get_test_from_testvalue_pair

    CombinationValue::const_iterator it = paircombo->begin();
    ++it;

    return *it;
}

bool is_else( const Value* value )
{
    // TODO: break this file into several (and test these functions?)

    const SymbolValue* sym = dynamic_cast<const SymbolValue*>( value );

    // TODO: case insensitive?
    return ( sym && sym->GetStringValue() == "else" );
}

const Value* process_cond( Evaluator* ev, const CombinationValue* combo,
    Environment& environment )
{
    // Look for pairs of predicate and value
    // or "else" and value, which must be last.

    if( combo->size() < 2 )
    {
        throw EvaluationError(
            "Not enough operands to cond: there should be at least 1 "
            "test-value pair." );
    }

    CombinationValue::const_iterator it = combo->begin();
    assert( it != combo->end() ); // "cond" - there are at least 2 items

    ++it;
    assert( it != combo->end() ); // test-value pair - there must be at least 1

    for( ; it != combo->end(); ++it )
    {
        const Value* test_value_pair = *it;

        const Value* test = get_test_from_testvalue_pair( test_value_pair );

        if( is_else( test ) )
        {
            ++it;
            if( it != combo->end() )
            {
                throw EvaluationError( "The else pair must be the last pair "
                    "in a 'cond' expression.  This value was found after the "
                    "else: '" + PrettyPrinter::Print( *it ) + "'." );
            }

            return get_value_from_testvalue_pair( test_value_pair );
        }
        else
        {
            std::auto_ptr<Value> evald_test = eval_in_context( ev, test,
                environment );

            if( is_true( evald_test.get() ) )
            {
                return get_value_from_testvalue_pair( test_value_pair );
            }
        }
    }

    // None of the conditions were true and there was no else - return NULL.
    return NULL;
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

std::auto_ptr<Value> eval_in_context( Evaluator* ev, const Value* value,
    Environment& environment )
{
    /*

    Apologies for the length of this function.  The logic of tail-call
    optimisation plus my desire for efficiency (i.e. direct returns rather
    than function calls followed by conditional returns) leaves me thinking
    that the only way I could break it up would be with gotos, which I think
    would damage rather than enhance comprehensibility.

    If anyone has any ideas how to break this up without damaging efficiency
    I would very much like to hear them.

    */

    EvalDepthMarker dm( ev->GetTracer() );

    // TODO: replace switch-style dispatch with a virtual method on Value

    auto_ptr<Value> new_value;
    auto_ptr<Environment> new_env;
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

        // Get hold of the operand
        CombinationValue::const_iterator cmbit = combo->begin();
        const Value* optr = *cmbit;

        // Check to see whether it's a special symbol
        const SymbolValue* sym = dynamic_cast<const SymbolValue*>( optr );
        if( sym )
        {
            const SymbolValue& symref = *sym;
            if( is_define_symbol( symref ) )
            {
                return eval_define( ev, combo, *env_ptr );
            }

            if( is_lambda_symbol( symref ) )
            {
                return eval_lambda( combo );
            }

            if( is_if_symbol( symref ) )
            {
                // If it's an if, we set value and go around the loop again
                // (tail-call optimisation)
                value = process_if( ev, combo, *env_ptr );
                continue;
            }

            if( is_cond_symbol( symref ) )
            {
                // If it's a cond, we set value and go around the loop again
                // (tail-call optimisation)
                value = process_cond( ev, combo, *env_ptr );
                if( value )
                {
                    continue;
                }
                else
                {
                    // If none of the cond tests were true, we got a NULL
                    // back, so we jump straight out here with a NULL too.
                    return auto_ptr<Value>( NULL );
                }
            }
        }

        // Otherwise we evaluate the operator
        auto_ptr<Value> evaldoptr = eval_in_context( ev, optr, *env_ptr );

        ++cmbit; // Skip past the procedure to the arguments

        // Evaluate the arguments into a new combo
        auto_ptr<CombinationValue> argvalues( new CombinationValue );
        for( ; cmbit != combo->end(); ++cmbit )
        {
            argvalues->push_back( eval_in_context( ev, *cmbit, *env_ptr
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
        new_env = proc->ExtendEnvironmentWithArgs( argvalues.get(),
            environment );
        env_ptr = new_env.get();

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
            eval_in_context( ev, *itbody, *env_ptr );
        }

        // Now we have the last bit of the body, which is the bit
        // we will tail-call optimise by setting value to it, and
        // going around the loop again.
        new_value.reset( (*itbody)->Clone() );
        value = new_value.get();
    }

}


}

Evaluator::Evaluator()
{
    tracer_ = &null_tracer_;
    BuiltIns::Init( global_environment_ );
}

std::auto_ptr<Value> Evaluator::Eval( const Value* value )
{
    // When the user entered the empty string, we have a NULL value.
    // In this case, we simply return NULL here.
    if( value )
    {
        return eval_in_context( this, value, global_environment_ );
    }
    else
    {
        return auto_ptr<Value>( NULL );
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

