
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


const CombinationValue* UserDefinedProcedureValue::GetBody() const
{
    return body_.get();
}


std::auto_ptr<Environment> UserDefinedProcedureValue::ExtendEnvironmentWithArgs(
    const CombinationValue* argvalues, const Environment& environment ) const
{
    // Set up an environment that extends the existing one
    std::auto_ptr<Environment> ret_environment( new Environment(
        environment, true ) );

    // Process the arguments, adding them to our environment
    CombinationValue::const_iterator itargname = argnames_->begin();
    CombinationValue::const_iterator itargvalue = argvalues->begin();
    for( ; itargname != argnames_->end(); ++itargname, ++itargvalue )
    {
        if( itargvalue == argvalues->end() )
        {
            ostringstream err;
            err << "Not enough arguments to procedure '" << name_
                << "'.  Expected "
                << argnames_->size() << " but got " << argvalues->size()
                << ".";
            throw EvaluationError( err.str() );
        }

        const SymbolValue* argsym = dynamic_cast<const SymbolValue*>(
            *itargname );
        if( !argsym )
        {
            throw EvaluationError( "The argument '"
                + PrettyPrinter::Print( *itargname )
                + "' in the definition of procedure '" + name_
                + "' is not a symbol." );
        }
        ret_environment->InsertSymbol( argsym->GetStringValue(),
            (*itargvalue)->Clone() );
    }

    if( itargvalue != argvalues->end() )
    {
        ostringstream err;
        err << "Too many arguments to procedure '" << name_ << "'.  Expected "
            << argnames_->size() << " but got " << argvalues->size() << ".";
        throw EvaluationError( err.str() );
    }

    return ret_environment;
}


