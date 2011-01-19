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
#include "lib/value/basic/decimalvalue.h"
#include "lib/value/basic/integervalue.h"
#include "lib/value/nativefunction/dividenativefunctionvalue.h"
#include "lib/value/value.h"
#include "lib/argschecker.h"
#include "lib/evaluationerror.h"
#include "lib/prettyprinter.h"

using namespace std;

namespace
{

const Value* get_numeric_value( CombinationValue::const_iterator& it )
{
    const Value* value = *it;
    const Value* result = dynamic_cast<const IntegerValue*>( value );
    if( !result )
    {
        result = dynamic_cast<const DecimalValue*>( value );
        if( !result )
        {
            throw EvaluationError( "Invalid argument for /: '"
                + PrettyPrinter::Print( value )
                + "' is not an integer or a decimal." );
        }
    }
    return result;
}

const DecimalValue* get_decimal_value( const Value* value )
{
    const DecimalValue* result = dynamic_cast<const DecimalValue*>( value );
    if( !result )
    {
        throw EvaluationError( "Invalid argument for /: '"
            + PrettyPrinter::Print( value )
            + "' is not an integer or a decimal." );
    }
    return result;
}

}

//virtual
std::auto_ptr<Value> DivideNativeFunctionValue::Run(
    EvaluationContext& ev, const CombinationValue* argvalues ) const
{
    auto_ptr<Value> result;

    CombinationValue::const_iterator it = argvalues->begin();

    if( it == argvalues->end() )
    {
        ArgsChecker::ThrowNotEnoughArgsException( "/", argvalues->size(), 1 );
    }

    if( argvalues->size() > 1 )
    {
        result.reset( get_numeric_value( it )->Clone() );
        ++it;
    }
    else
    {
        result.reset( new IntegerValue( 1 ) );
    }

    for( ; it != argvalues->end(); ++it )
    {
        const Value* operand = *it;

        // TODO: some kind of double dispatch?
        const IntegerValue* intop = dynamic_cast<const IntegerValue*>(
            operand );
        const IntegerValue* intres = dynamic_cast<const IntegerValue*>(
            result.get() );

        if( intres )
        {
            if( intop )
            {
                result = *intres / *intop;
            }
            else
            {
                result = *intres / *get_decimal_value( operand );
            }
        }
        else
        {
            DecimalValue* decres = dynamic_cast<DecimalValue*>( result.get() );
            assert( decres ); // result must be a decimal if it's not an int

            if( intop )
            {
                *decres /= *intop;
            }
            else
            {
                *decres /= *get_decimal_value( operand );
            }
        }
    }

    return result;
}


//virtual
DivideNativeFunctionValue* DivideNativeFunctionValue::Clone() const
{
    return new DivideNativeFunctionValue( *this );
}


//virtual
std::string DivideNativeFunctionValue::GetName() const
{
    return StaticName();
}

//static
const std::string& DivideNativeFunctionValue::StaticName()
{
    static const string static_name( "/" );
    return static_name;
}


