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
#include <memory>

#include "argschecker.h"
#include "combinationvalue.h"
#include "evaluationerror.h"
#include "evaluator.h"
#include "prettyprinter.h"
#include "symbolvalue.h"
#include "value.h"

#include "compoundprocedurevalue.h"

using namespace std;

CompoundProcedureValue::CompoundProcedureValue(
    CombinationValue* argnames,
    CombinationValue* body,
    const std::string& name )
: argnames_( argnames )
, body_( body )
, name_( name )
{
    // TODO: validate that each of the elements of args is a symbol?
}

CompoundProcedureValue::CompoundProcedureValue(
    const CompoundProcedureValue& other )
: argnames_( other.argnames_->Clone() )
, body_( other.body_->Clone() )
, name_( other.name_ )
{
}


//virtual
CompoundProcedureValue* CompoundProcedureValue::Clone() const
{
    return new CompoundProcedureValue( *this );
}


//virtual
std::string CompoundProcedureValue::GetName() const
{
    string ret = "COMPOUND-PROCEDURE";
    if( !name_.empty() )
    {
        ret += " ";
        ret += name_;
    }
    return ret;
}


const CombinationValue* CompoundProcedureValue::GetBody() const
{
    return body_.get();
}


std::auto_ptr<Environment> CompoundProcedureValue::ExtendEnvironmentWithArgs(
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
            ArgsChecker::ThrowWrongNumArgsException( name_.c_str(),
                argvalues->size(), argnames_->size() );
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
        ArgsChecker::ThrowWrongNumArgsException( name_.c_str(),
            argvalues->size(), argnames_->size() );
    }

    return ret_environment;
}


