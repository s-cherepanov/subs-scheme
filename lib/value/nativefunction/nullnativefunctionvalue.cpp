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
#include <string>

#include "lib/argschecker.h"
#include "lib/combinationvalue.h"
#include "lib/nilvalue.h"

#include "nullnativefunctionvalue.h"

using namespace std;

//virtual
std::auto_ptr<Value> NullNativeFunctionValue::Run(
    const CombinationValue* argvalues ) const
{
    ArgsChecker::CheckExactNumberOfArgs( GetName().c_str(), argvalues, 1 );

    const Value* value = *( argvalues->begin() );
    return CreateBooleanValue( dynamic_cast<const NilValue*>( value ) );
}


//virtual
NullNativeFunctionValue* NullNativeFunctionValue::Clone() const
{
    return new NullNativeFunctionValue( *this );
}


//virtual
std::string NullNativeFunctionValue::GetName() const
{
    return StaticName();
}

//static
const std::string& NullNativeFunctionValue::StaticName()
{
    static const string static_name( "null?" );
    return static_name;
}


