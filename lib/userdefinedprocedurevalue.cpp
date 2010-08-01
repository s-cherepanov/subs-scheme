
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


Environment extend_environment_with_args(
    const CombinationValue* argnames,
    const CombinationValue* combo,
    const Environment& environment )
{
    // Set up an environment that extends the existing one
    Environment ret_environment( environment, true );

    CombinationValue::const_iterator itargvalue = combo->begin();
    // We have at least one thing in combo - the operator
    assert( itargvalue != combo->end() );
    // Skip the operator
    ++itargvalue;
    
    // Process the arguments, adding them to our environment
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
        ret_environment.InsertSymbol( argsym->GetStringValue(),
            (*itargvalue)->Clone() );
    }

    if( itargvalue != combo->end() )
    {
        ostringstream err;
        err << "Too many arguments to procedure.  Expected "
            << argnames->size() << " but got " << combo->size() - 1 << ".";
        throw EvaluationError( err.str() );
    }
    return ret_environment;
}


}

UserDefinedProcedureValue::UserDefinedProcedureValue(
    CombinationValue* argnames,
    CombinationValue* body,
    const std::string& name )
: argnames_( argnames )
, body_( body )
, name_( name )
{
    // TODO: validate that each of the elements of args is a symbol?
}

UserDefinedProcedureValue::UserDefinedProcedureValue(
    const UserDefinedProcedureValue& other )
: argnames_( other.argnames_->Clone() )
, body_( other.body_->Clone() )
, name_( other.name_ )
{
}

//virtual
std::auto_ptr<Value> UserDefinedProcedureValue::Run(
    Evaluator* ev, const CombinationValue* combo, Environment& environment )
{
    auto_ptr<Value> ret;

    // Create our own environment for running this procedure
    Environment new_environment = extend_environment_with_args(
        argnames_.get(), combo, environment );

    for( CombinationValue::const_iterator it = body_->begin();
        it != body_->end(); ++it )
    {
        ret = ev->EvalInContext( *it, new_environment );
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
    string ret = "user-defined";
    if( !name_.empty() )
    {
        ret += ": ";
        ret += name_;
    }
    return ret;
}


