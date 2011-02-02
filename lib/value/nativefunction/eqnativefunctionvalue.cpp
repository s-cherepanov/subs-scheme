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
#include <memory>
#include <string>

#include "lib/value/basic/combinationvalue.h"
#include "lib/value/symbol/symbolvalue.h"
#include "lib/value/nativefunction/eqnativefunctionvalue.h"
#include "lib/argschecker.h"
#include "lib/evaluationerror.h"
#include "lib/prettyprinter.h"

using namespace std;

namespace
{

bool eq( const CombinationValue* argvalues )
{
    ArgsChecker::CheckExactNumberOfArgs( "eq?", argvalues, 2 );

    CombinationValue::const_iterator it = argvalues->begin();

    assert( it != argvalues->end() );

    const SymbolValue* sym1 = dynamic_cast<const SymbolValue*>( *it );
    if( !sym1 )
    {
        return false;
    }

    ++it;
    assert( it != argvalues->end() );

    const SymbolValue* sym2 = dynamic_cast<const SymbolValue*>( *it );
    if( !sym2 )
    {
        return false;
    }

    return *sym1 == *sym2;
}

}

//virtual
std::auto_ptr<Value> EqNativeFunctionValue::Run(
    EvaluationContext& ev, const CombinationValue* argvalues ) const
{
    return CreateBooleanValue( eq( argvalues ) );
}


//virtual
EqNativeFunctionValue* EqNativeFunctionValue::Clone() const
{
    return new EqNativeFunctionValue( *this );
}


//virtual
std::string EqNativeFunctionValue::GetName() const
{
    return StaticName();
}

//static
const std::string& EqNativeFunctionValue::StaticName()
{
    static const string static_name( "eq?" );
    return static_name;
}


