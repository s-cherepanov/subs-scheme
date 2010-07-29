
#include <cassert>
#include <iostream>

#include "combinationvalue.h"
#include "evaluationerror.h"
#include "plusprocedurevalue.h"
#include "procedurevalue.h"
#include "prettyprinter.h"
#include "symbolvalue.h"
#include "value.h"

#include "evaluator.h"

using namespace std;

namespace
{


std::auto_ptr<Value> apply( const CombinationValue* combo )
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

    return operator_proc->Run( combo );
}

bool is_define_symbol( const Value* value )
{
    const SymbolValue* sym = dynamic_cast<const SymbolValue*>( value );
    if( sym )
    {
        // TODO: case insensitive?
        if( sym->GetStringValue() == "define" )
        {
            return true;
        }
    }
    return false;
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

std::auto_ptr<Value> eval_combo( Evaluator* ev, const CombinationValue* combo )
{
    auto_ptr<CombinationValue> new_combo( new CombinationValue );
    for( CombinationValue::const_iterator it = combo->begin();
        it != combo->end(); ++it )
    {
        new_combo->push_back( ev->Eval( *it ).release() );
    }

    return apply( new_combo.get() );

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
        if( combo->begin() != combo->end() && is_define_symbol(
            *combo->begin() ) )
        {
            return eval_define( this, combo );
        }
        else
        {
            return eval_combo( this, combo );
        }
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


