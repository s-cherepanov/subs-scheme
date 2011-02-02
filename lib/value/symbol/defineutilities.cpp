/*
 *  Subs Scheme Interpreter
 *  Copyright (C) 2010, 2011 Andy Balaam
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

#include <memory>
#include <string>
#include <boost/shared_ptr.hpp>

#include "lib/value/basic/combinationvalue.h"
#include "lib/value/basic/compoundprocedurevalue.h"
#include "lib/value/symbol/defineutilities.h"
#include "lib/value/value.h"
#include "lib/environment.h"
#include "lib/evaluator.h"

using namespace std;

namespace
{

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

}

namespace DefineUtilities
{


std::auto_ptr<Value> define_procedure(
    CombinationValue::const_iterator itarg,
    const CombinationValue::const_iterator& argsend,
    CombinationValue::const_iterator itbody,
    const CombinationValue::const_iterator& bodyend,
    const boost::shared_ptr<Environment>& environment,
    const std::string& name /*=""*/ )
{
    return auto_ptr<Value>( new CompoundProcedureValue(
        clone_partial_combo( itarg, argsend ).release(),
        clone_partial_combo( itbody, bodyend ).release(), name, environment ) );
}


void insert_value_into_environment( boost::shared_ptr<Environment>& environment,
    const std::string& name, std::auto_ptr<Value>& value )
{
    CompoundProcedureValue* proc = dynamic_cast<CompoundProcedureValue*>(
        value.get() );

    if( proc )
    {
        proc->NotifyBeingInsertedInto( *environment );
    }

    environment->InsertSymbol( name, value.release() );
}


}

