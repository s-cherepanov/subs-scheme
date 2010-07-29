
#include <cassert>
#include <iostream>

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


std::auto_ptr<Value> apply( Evaluator* ev, const CombinationValue* combo )
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

    return operator_proc->Run( ev, combo );
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

std::auto_ptr<Value> eval_define( Evaluator* ev, const CombinationValue* combo )
{
    if( combo->size() < 3 )
    {
        throw EvaluationError(
            "Too few operands to the define operator: there should be 2." );
    }

    if( combo->size() > 3 )
    {
        throw EvaluationError(
            "Too many operands to the define operator: there should be 2." );
    }

    CombinationValue::const_iterator it = combo->begin();
    assert( it != combo->end() );
    ++it;
    assert( it != combo->end() );

    const SymbolValue* sym = dynamic_cast<const SymbolValue*>( *it );
    if( !sym )
    {
        throw EvaluationError( "The first operand to the define operator "
            "must be a symbol.  (Note we don't yet support definining "
            "procedures.)" );
    }

    ++it;
    assert( it != combo->end() );

    // TODO: not always global
    SymbolValueMap& symbols = ev->GetGlobalSymbolMap();
    symbols[ sym->GetStringValue() ] = ev->Eval( *it ).release();

    return auto_ptr<Value>( sym->Clone() );
}


std::auto_ptr<Value> eval_lambda( Evaluator* ev, const CombinationValue* combo )
{
    if( combo->size() < 3 )
    {
        throw EvaluationError(
            "Too few operands to the lambda operator: there should "
            "be  at least 2." );
    }

    CombinationValue::const_iterator it = combo->begin();
    assert( it != combo->end() );
    ++it;
    assert( it != combo->end() );

    const CombinationValue* args = dynamic_cast<const CombinationValue*>( *it );
    if( !args )
    {
        throw EvaluationError( "The first operand to the lambda operator "
            "must be a combination naming its arguments.  '"
            + PrettyPrinter::Print( *it )
            + "' is not a combination." );
    }

    ++it;
    assert( it != combo->end() );

    // Copy the rest of the combo (unevaluated) as the
    // body of the function
    auto_ptr<CombinationValue> bodycombo( new CombinationValue );
    for( ; it != combo->end(); ++it )
    {
        bodycombo->push_back( (*it)->Clone() );
    }
    
    return auto_ptr<Value>( new UserDefinedProcedureValue( args->Clone(),
        bodycombo.release() ) );
}


std::auto_ptr<Value> eval_combo( Evaluator* ev, const CombinationValue* combo )
{
    auto_ptr<CombinationValue> new_combo( new CombinationValue );
    for( CombinationValue::const_iterator it = combo->begin();
        it != combo->end(); ++it )
    {
        new_combo->push_back( ev->Eval( *it ).release() );
    }

    return apply( ev, new_combo.get() );

}

std::auto_ptr<Value> eval_symbol( Evaluator* ev, const SymbolValue* sym )
{
    const SymbolValueMap& symbols = ev->GetGlobalSymbolMap();
    SymbolValueMap::const_iterator itFind = symbols.find(
        sym->GetStringValue() );

    if( itFind == symbols.end() )
    {
        throw EvaluationError( "Undefined symbol '"
            + PrettyPrinter::Print( sym ) + "'." );
    }

    // TODO: avoid the copy here?
    return auto_ptr<Value>( itFind->second->Clone() );
}


}

Evaluator::Evaluator()
: print_intermediates_( true )
{
    global_symbols_["+"] = new PlusProcedureValue();
}

std::auto_ptr<Value> Evaluator::Eval( const Value* value )
{
    // TODO: replace switch-style dispatch with a virtual method on Value

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
                    return eval_define( this, combo );
                }

                if( is_lambda_symbol( sym ) )
                {
                    return eval_lambda( this, combo );
                }
            }
        }

        // If none of the special cases occurred, treat it as
        // a normal combination
        return eval_combo( this, combo );
    }

    const SymbolValue* sym = dynamic_cast<const SymbolValue*>(
        value );

    if( sym )
    {
        return eval_symbol( this, sym );
    }

    return auto_ptr<Value>( value->Clone() );
}

const SymbolValueMap& Evaluator::GetGlobalSymbolMap() const
{
    return global_symbols_;
}

SymbolValueMap& Evaluator::GetGlobalSymbolMap()
{
    return global_symbols_;
}


