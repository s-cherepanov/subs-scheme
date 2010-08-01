
#include <cassert>

#include "combinationvalue.h"
#include "evaluationerror.h"
#include "plusprocedurevalue.h"
#include "procedurevalue.h"
#include "prettyprinter.h"
#include "symbolvalue.h"
#include "userdefinedprocedurevalue.h"
#include "value.h"

#include "evaluator.h"

using namespace std;

namespace
{


std::auto_ptr<Value> apply( Evaluator* ev, const CombinationValue* combo,
    Environment& environment )
{
    if( combo->empty() )
    {
        // TODO: supply line number, file etc.
        throw EvaluationError( "Attepted to evaluate an empty combination" );
    }

    ProcedureValue* operator_proc = dynamic_cast<ProcedureValue*>(
        (*combo)[0] );

    if( !operator_proc )
    {
        throw EvaluationError( "Attempted to run procedure '"
            + PrettyPrinter::Print( (*combo)[0] )
            + "', which is not a symbol." );
    }

    return operator_proc->Run( ev, combo, environment );
}

bool is_define_symbol( const SymbolValue* sym )
{
    // TODO: case insensitive?
    return ( sym->GetStringValue() == "define" );
}


bool is_lambda_symbol( const SymbolValue* sym )
{
    // TODO: case insensitive?
    return ( sym->GetStringValue() == "lambda" );
}


std::auto_ptr<Value> eval_define_symbol( Evaluator* ev,
    Environment& environment, const SymbolValue* to_define,
    const Value* definition )
{
    environment.InsertSymbol( to_define->GetStringValue(),
        ev->EvalInContext( definition, environment ).release() );

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
    return auto_ptr<Value>( new UserDefinedProcedureValue(
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


std::auto_ptr<Value> eval_combo( Evaluator* ev, const CombinationValue* combo,
    Environment& environment )
{
    auto_ptr<CombinationValue> new_combo( new CombinationValue );
    for( CombinationValue::const_iterator it = combo->begin();
        it != combo->end(); ++it )
    {
        new_combo->push_back( ev->EvalInContext( *it, environment ).release() );
    }

    return apply( ev, new_combo.get(), environment );

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


}

Evaluator::Evaluator()
{
    global_environment_.InsertSymbol( "+", new PlusProcedureValue() );
}

std::auto_ptr<Value> Evaluator::Eval( const Value* value )
{
    return EvalInContext( value, global_environment_ );
}

std::auto_ptr<Value> Evaluator::EvalInContext( const Value* value,
    Environment& environment )
{
    // TODO: replace switch-style dispatch with a virtual method on Value

    // When the user entered the empty string, we have a NULL value
    if( !value )
    {
        return auto_ptr<Value>( NULL );
    }

    const CombinationValue* combo = dynamic_cast<const CombinationValue*>(
        value );
    if( combo )
    {
        // Check for special keywords

        if( combo->begin() != combo->end() )
        {
            const SymbolValue* sym = dynamic_cast<const SymbolValue*>(
                (*combo)[0] );

            if( sym )
            {
                if( is_define_symbol( sym ) )
                {
                    return eval_define( this, combo, environment );
                }

                if( is_lambda_symbol( sym ) )
                {
                    return eval_lambda( combo );
                }
            }
        }

        // If none of the special cases occurred, treat it as
        // a normal combination
        return eval_combo( this, combo, environment );
    }

    const SymbolValue* sym = dynamic_cast<const SymbolValue*>(
        value );

    if( sym )
    {
        return eval_symbol( sym, environment );
    }

    return auto_ptr<Value>( value->Clone() );
}


