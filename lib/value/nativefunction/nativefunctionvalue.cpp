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

#include <cassert>

#include "lib/value/basic/combinationvalue.h"
#include "lib/value/basic/decimalvalue.h"
#include "lib/value/basic/falsevalue.h"
#include "lib/value/basic/integervalue.h"
#include "lib/value/basic/truevalue.h"
#include "lib/value/nativefunction/nativefunctionvalue.h"
#include "lib/argschecker.h"
#include "lib/evaluationerror.h"
#include "lib/prettyprinter.h"

//protected
double NativeFunctionValue::GetDoubleArg( const CombinationValue* argvalues )
    const
{
    ArgsChecker::CheckExactNumberOfArgs( GetName().c_str(), argvalues, 1 );

    CombinationValue::const_iterator it = argvalues->begin();
    assert( it != argvalues->end() );

    double value = 0;
    const DecimalValue* operand_dec = dynamic_cast<const DecimalValue*>( *it );
    if( operand_dec )
    {
        value = operand_dec->GetDoubleValue();
    }
    else
    {
        const IntegerValue* operand_int = dynamic_cast<const IntegerValue*>(
            *it );
        if( !operand_int )
        {
            throw EvaluationError( "Invalid argument for "
                + GetName()
                + ": '"
                + PrettyPrinter::Print( *it )
                + "' is not an integer or a decimal." );
        }
        value = operand_int->GetIntValue();
    }

    return value;
}

//protected
int NativeFunctionValue::GetConvertibleToIntArg(
    const CombinationValue* argvalues ) const
{
    ArgsChecker::CheckExactNumberOfArgs( GetName().c_str(), argvalues, 1 );

    CombinationValue::const_iterator it = argvalues->begin();
    assert( it != argvalues->end() );

    Value* value = *it;

    const IntegerValue* ivalue = dynamic_cast<const IntegerValue*>( value );

    if( ivalue )
    {
        return ivalue->GetIntValue();
    }

    const DecimalValue* dvalue = dynamic_cast<const DecimalValue*>( value );

    if( !dvalue )
    {
        throw EvaluationError( "Invalid argument for "
                + GetName()
                + ": '"
                + PrettyPrinter::Print( value )
                + "' is not an integer or a decimal." );
    }

    bool was_int;
    int i = dvalue->AsExactInt( was_int );
    if( !was_int )
    {
        throw EvaluationError( "Invalid argument for "
            + GetName()
            + ": '"
            + PrettyPrinter::Print( value )
            + "' is a decimal that does not have an integer value." );
    }

    return i;
}


//protected static
std::auto_ptr<Value> NativeFunctionValue::CreateBooleanValue( bool value )
{
    if( value )
    {
        return std::auto_ptr<Value>( new TrueValue );
    }
    else
    {
        return std::auto_ptr<Value>( new FalseValue );
    }
}

