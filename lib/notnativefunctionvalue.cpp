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

#include <memory>
#include <sstream>

#include "combinationvalue.h"
#include "evaluationerror.h"
#include "falsevalue.h"
#include "notnativefunctionvalue.h"
#include "prettyprinter.h"
#include "truevalue.h"
#include "value.h"
#include "valueutilities.h"

using namespace std;

//virtual
std::auto_ptr<Value> NotNativeFunctionValue::Run(
    const CombinationValue* argvalues ) const
{
    if( argvalues->size() != 1 )
    {
        if( argvalues->empty() )
        {
            throw EvaluationError( "Not enough operands to not: there "
                "should be 1, but there were none." );
        }
        else
        {
            ostringstream ss;
            ss << "Too many operands to not: there were ";
            ss << argvalues->size();
            ss << " but there should be 1.";
            throw EvaluationError( ss.str() );
        }
    }

    CombinationValue::const_iterator it = argvalues->begin();

    if( ValueUtilities::IsFalse( *it ) )
    {
        return auto_ptr<Value>( new TrueValue );
    }
    else
    {
        return auto_ptr<Value>( new FalseValue );
    }
}


//virtual
NotNativeFunctionValue* NotNativeFunctionValue::Clone() const
{
    return new NotNativeFunctionValue( *this );
}


//virtual
std::string NotNativeFunctionValue::GetName() const
{
    return StaticName();
}

//static
const std::string& NotNativeFunctionValue::StaticName()
{
    static const string static_name( "not" );
    return static_name;
}


