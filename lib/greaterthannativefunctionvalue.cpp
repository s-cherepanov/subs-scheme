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

#include "combinationvalue.h"
#include "evaluationerror.h"
#include "falsevalue.h"
#include "integervalue.h"
#include "prettyprinter.h"
#include "truevalue.h"
#include "value.h"

#include "greaterthannativefunctionvalue.h"

using namespace std;

//virtual
std::auto_ptr<Value> GreaterThanNativeFunctionValue::Run(
    const CombinationValue* argvalues ) const
{
    // TODO: values other than integers

    CombinationValue::const_iterator it = argvalues->begin();

    if( it == argvalues->end() )
    {
        throw EvaluationError( "Not enough operands to >: there should "
            "be at least 2." );
    }

    const IntegerValue* previous = dynamic_cast<const IntegerValue*>( *it );

    if( !previous )
    {
        throw EvaluationError( "Wrong operand type for >: an integer was "
            "expected, but '" + PrettyPrinter::Print( *it )
            + "' is not an integer." );
    }

    ++it;

    if( it == argvalues->end() )
    {
        throw EvaluationError( "Not enough operands to >: there should "
            "be at least 2." );
    }

    for( ; it != argvalues->end(); ++it )
    {
        const IntegerValue* operand = dynamic_cast<IntegerValue*>( *it );
        if( !operand )
        {
            // TODO: consistent error messages for all builtins
            throw EvaluationError( "Wrong operand type for >: an integer was "
                " expected, but '" + PrettyPrinter::Print( *it )
                + "' is not an integer." );
        }

        if( *previous <= *operand )
        {
            return auto_ptr<Value>( new FalseValue );
        }
        previous = operand;
    }

    return auto_ptr<Value>( new TrueValue );
}


//virtual
GreaterThanNativeFunctionValue* GreaterThanNativeFunctionValue::Clone() const
{
    return new GreaterThanNativeFunctionValue( *this );
}


//virtual
std::string GreaterThanNativeFunctionValue::GetName() const
{
    return StaticName();
}

//static
const std::string& GreaterThanNativeFunctionValue::StaticName()
{
    static const string static_name( ">" );
    return static_name;
}


