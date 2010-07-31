
#include <cassert>
#include <sstream>
#include <memory>

#include "combinationvalue.h"
#include "evaluationerror.h"
#include "evaluator.h"
#include "prettyprinter.h"
#include "symbolvalue.h"
#include "value.h"

#include "userdefinedprocedurevalue.h"

using namespace std;

namespace
{


void process_args(
    const CombinationValue* argnames,
    const CombinationValue* combo,
    SymbolValueMap& symbols )
{
    CombinationValue::const_iterator itargvalue = combo->begin();
    // We have at least one thing in combo - the operator
    assert( itargvalue != combo->end() );
    // Skip the operator
    ++itargvalue;
    
    // Process the arguments, adding them to our symbols table
    for( CombinationValue::const_iterator itargname = argnames->begin();
        itargname != argnames->end(); ++itargname, ++itargvalue )
    {
        if( itargvalue == combo->end() )
        {
            ostringstream err;
            err << "Not enough arguments to procedure.  Expected "
                << argnames->size() << " but got " << combo->size() - 1 << ".";
            throw EvaluationError( err.str() );
        }

        const SymbolValue* argsym = dynamic_cast<const SymbolValue*>(
            *itargname );
        if( !argsym )
        {
            throw EvaluationError( "The argument '"
                + PrettyPrinter::Print( *itargname )
                + "' in a lambda expression is not a symbol." );
        }
        symbols[ argsym->GetStringValue() ] = (*itargvalue)->Clone();
    }

    if( itargvalue != combo->end() )
    {
        ostringstream err;
        err << "Too many arguments to procedure.  Expected "
            << argnames->size() << " but got " << combo->size() - 1 << ".";
        throw EvaluationError( err.str() );
    }
}


}

UserDefinedProcedureValue::UserDefinedProcedureValue(
    CombinationValue* argnames,
    CombinationValue* body )
: argnames_( argnames )
, body_( body )
{
    // TODO: validate that each of the elements of args is a symbol?
}

UserDefinedProcedureValue::UserDefinedProcedureValue(
    const UserDefinedProcedureValue& other )
: argnames_( other.argnames_->Clone() )
, body_( other.body_->Clone() )
{
}

//virtual
std::auto_ptr<Value> UserDefinedProcedureValue::Run(
    Evaluator* ev, const CombinationValue* combo )
{
    auto_ptr<Value> ret;

    // TODO: don't pollute the global namespace
    SymbolValueMap& symbols = ev->GetGlobalSymbolMap();

    process_args( argnames_.get(), combo, symbols );

    for( CombinationValue::const_iterator it = body_->begin();
        it != body_->end(); ++it )
    {
        ret = ev->Eval( *it );
    }

    return ret;
}


//virtual
UserDefinedProcedureValue* UserDefinedProcedureValue::Clone() const
{
    return new UserDefinedProcedureValue( *this );
}


//virtual
std::string UserDefinedProcedureValue::GetName() const
{
    return "user-defined";
}


