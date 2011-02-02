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

#include "lib/value/nativefunction/negativenativefunctionvalue.h"

using namespace std;

//virtual
std::auto_ptr<Value> NegativeNativeFunctionValue::Run(
    EvaluationContext& ev, const CombinationValue* argvalues ) const
{
    double value = GetDoubleArg( argvalues );
    return CreateBooleanValue( value < 0 );
}


//virtual
NegativeNativeFunctionValue* NegativeNativeFunctionValue::Clone() const
{
    return new NegativeNativeFunctionValue( *this );
}


//virtual
std::string NegativeNativeFunctionValue::GetName() const
{
    return StaticName();
}

//static
const std::string& NegativeNativeFunctionValue::StaticName()
{
    static const string static_name( "negative?" );
    return static_name;
}


