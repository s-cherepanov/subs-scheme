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
#include <sstream>

#include "lib/value/basic/combinationvalue.h"
#include "lib/value/basic/falsevalue.h"
#include "lib/value/basic/truevalue.h"
#include "lib/value/nativefunction/notnativefunctionvalue.h"
#include "lib/value/value.h"
#include "lib/argschecker.h"
#include "lib/evaluationerror.h"
#include "lib/prettyprinter.h"
#include "lib/valueutilities.h"

using namespace std;

//virtual
std::auto_ptr<Value> NotNativeFunctionValue::Run(
    EvaluationContext& ev, const CombinationValue* argvalues ) const
{
    ArgsChecker::CheckExactNumberOfArgs( "not", argvalues, 1 );

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


