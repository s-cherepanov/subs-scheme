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
#include "decimalvalue.h"
#include "evaluationerror.h"
#include "falsevalue.h"
#include "integervalue.h"
#include "prettyprinter.h"
#include "truevalue.h"
#include "value.h"

#include "lessthannativefunctionvalue.h"

using namespace std;

//virtual
std::auto_ptr<Value> LessThanNativeFunctionValue::Run(
    const CombinationValue* argvalues ) const
{
    CombinationValue::const_iterator it = argvalues->begin();

    if( it == argvalues->end() )
    {
        throw EvaluationError( "Not enough operands to <: there should "
            "be at least 2." );
    }

    const IntegerValue* previous_int = dynamic_cast<const IntegerValue*>( *it );
    const DecimalValue* previous_dec = NULL;

    if( !previous_int )
    {
        previous_dec = dynamic_cast<const DecimalValue*>( *it );
        if( !previous_dec )
        {
            throw EvaluationError( "Wrong operand type for <: a number was "
                "expected, but '" + PrettyPrinter::Print( *it )
                + "' is not a number." );
        }
    }

    ++it;

    if( it == argvalues->end() )
    {
        throw EvaluationError( "Not enough operands to <: there should "
            "be at least 2." );
    }

    for( ; it != argvalues->end(); ++it )
    {
        const IntegerValue* operand_int = dynamic_cast<const IntegerValue*>(
            *it );
        const DecimalValue* operand_dec = NULL;
        if( !operand_int )
        {
            operand_dec = dynamic_cast<const DecimalValue*>( *it );
            if( !operand_dec )
            {
                throw EvaluationError( "Wrong operand type for <: a number "
                    "was expected, but '" + PrettyPrinter::Print( *it )
                    + "' is not a number." );
            }
        }

        if( previous_int )
        {
            if( operand_int )
            {
                if( *previous_int >= *operand_int )
                {
                    return auto_ptr<Value>( new FalseValue );
                }
                previous_int = operand_int;
                previous_dec = NULL;
            }
            else
            {
                if( *previous_int >= *operand_dec )
                {
                    return auto_ptr<Value>( new FalseValue );
                }
                previous_int = NULL;
                previous_dec = operand_dec;
            }
        }
        else
        {
            if( operand_int )
            {
                if( *previous_dec >= *operand_int )
                {
                    return auto_ptr<Value>( new FalseValue );
                }
                previous_int = operand_int;
                previous_dec = NULL;
            }
            else
            {
                if( *previous_dec >= *operand_dec )
                {
                    return auto_ptr<Value>( new FalseValue );
                }
                previous_int = NULL;
                previous_dec = operand_dec;
            }
        }
    }

    return auto_ptr<Value>( new TrueValue );
}


//virtual
LessThanNativeFunctionValue* LessThanNativeFunctionValue::Clone() const
{
    return new LessThanNativeFunctionValue( *this );
}


//virtual
std::string LessThanNativeFunctionValue::GetName() const
{
    return StaticName();
}

//static
const std::string& LessThanNativeFunctionValue::StaticName()
{
    static const string static_name( "<" );
    return static_name;
}


