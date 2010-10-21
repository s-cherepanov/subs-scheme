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

#include "lib/value/basic/combinationvalue.h"
#include "lib/value/basic/falsevalue.h"
#include "lib/value/basic/integervalue.h"
#include "lib/value/basic/truevalue.h"
#include "lib/value/nativefunction/equalsnativefunctionvalue.h"
#include "lib/argschecker.h"
#include "lib/evaluationerror.h"
#include "lib/prettyprinter.h"
#include "lib/value.h"


using namespace std;

//virtual
std::auto_ptr<Value> EqualsNativeFunctionValue::Run(
    const CombinationValue* argvalues ) const
{
    // TODO: values other than integers

    CombinationValue::const_iterator it = argvalues->begin();

    if( it == argvalues->end() )
    {
        ArgsChecker::ThrowNotEnoughArgsException( "=", argvalues->size(), 2 );
    }

    const IntegerValue* first = dynamic_cast<const IntegerValue*>( *it );

    if( !first )
    {
        throw EvaluationError( "Wrong operand type for =: an integer was "
            "expected, but '" + PrettyPrinter::Print( *it )
            + "' is not an integer." );
    }

    ++it;

    if( it == argvalues->end() )
    {
        ArgsChecker::ThrowNotEnoughArgsException( "=", argvalues->size(), 2 );
    }

    for( ; it != argvalues->end(); ++it )
    {
        const IntegerValue* operand = dynamic_cast<IntegerValue*>( *it );
        if( !operand )
        {
            // TODO: consistent error messages for all builtins
            throw EvaluationError( "Wrong operand type for =: an integer was "
                " expected, but '" + PrettyPrinter::Print( *it )
                + "' is not an integer." );
        }

        if( *first != *operand )
        {
            return auto_ptr<Value>( new FalseValue );
        }
    }

    return auto_ptr<Value>( new TrueValue );
}


//virtual
EqualsNativeFunctionValue* EqualsNativeFunctionValue::Clone() const
{
    return new EqualsNativeFunctionValue( *this );
}


//virtual
std::string EqualsNativeFunctionValue::GetName() const
{
    return StaticName();
}

//static
const std::string& EqualsNativeFunctionValue::StaticName()
{
    static const string static_name( "=" );
    return static_name;
}


