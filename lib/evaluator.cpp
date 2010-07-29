
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

Evaluator::Evaluator()
: print_intermediates_( true )
{
    global_symbols_["+"] = new PlusProcedureValue();
}

std::auto_ptr<Value> Evaluator::Eval( const Value* value )
{
    const CombinationValue* combo = dynamic_cast<const CombinationValue*>(
        value );
    if( combo )
    {
        auto_ptr<CombinationValue> new_combo( new CombinationValue );
        for( CombinationValue::const_iterator it = combo->begin();
            it != combo->end(); ++it )
        {
            auto_ptr<Value> new_item = Eval( *it );
            if( new_item.get() )
            {
                new_combo->push_back( new_item.release() );
            }
            else
            {
                new_combo->push_back( (*it)->Clone() );
            }
        }

        return Apply( new_combo.get() );
    }
    else
    {
        const SymbolValue* sym = dynamic_cast<const SymbolValue*>(
            value );

        if( sym )
        {
            SymbolValueMap::const_iterator itFind = global_symbols_.find(
                sym->GetStringValue() );

            if( itFind == global_symbols_.end() )
            {
                throw EvaluationError( "Undefined symbol '"
                    + PrettyPrinter::Print( sym ) + "'." );
            }

            return auto_ptr<Value>( itFind->second->Clone() );
        }
        else
        {
            return auto_ptr<Value>( NULL );
        }

    }
}

std::auto_ptr<Value> Evaluator::Apply( const CombinationValue* combo )
{
    if( combo->empty() )
    {
        // TODO: supply line number, file etc.
        throw EvaluationError( "Attepted to evaluate an empty comination" );
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


