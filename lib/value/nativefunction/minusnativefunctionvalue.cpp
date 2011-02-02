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
#include <memory>

#include "lib/value/basic/combinationvalue.h"
#include "lib/value/basic/decimalvalue.h"
#include "lib/value/basic/integervalue.h"
#include "lib/value/nativefunction/minusnativefunctionvalue.h"
#include "lib/value/value.h"
#include "lib/argschecker.h"
#include "lib/evaluationerror.h"
#include "lib/prettyprinter.h"

using namespace std;

namespace
{

void run_decimal( auto_ptr<DecimalValue>& result,
    CombinationValue::const_iterator it, const CombinationValue* argvalues )
{
    for( ; it != argvalues->end(); ++it )
    {
        const DecimalValue* decop = dynamic_cast<DecimalValue*>( *it );
        if( decop )
        {
            *result -= *decop;
        }
        else
        {
            const IntegerValue* intop = dynamic_cast<IntegerValue*>( *it );
            if( !intop )
            {
                throw EvaluationError( "Invalid argument for -: '"
                    + PrettyPrinter::Print( *it )
                    + "' is not an integer or a decimal." );
            }
            else
            {
                *result -= *intop;
            }
        }
    }
}

std::auto_ptr<Value> run_decimal_from_start(
    CombinationValue::const_iterator it, const CombinationValue* argvalues )
{
    const DecimalValue* decres = dynamic_cast<const DecimalValue*>( *it );

    if( !decres )
    {
        throw EvaluationError( "Invalid argument for -: '"
            + PrettyPrinter::Print( *it )
            + "' is not an integer or a decimal." );
    }

    auto_ptr<DecimalValue> result( new DecimalValue( *decres ) );
    ++it;

    run_decimal( result, it, argvalues );

    return auto_ptr<Value>( result.release() );
}

std::auto_ptr<Value> run_decimal_from_int( const IntegerValue* intres,
    CombinationValue::const_iterator it, const CombinationValue* argvalues )
{
    auto_ptr<DecimalValue> result( new DecimalValue( *intres ) );

    run_decimal( result, it, argvalues );

    return auto_ptr<Value>( result.release() );
}

}

//virtual
std::auto_ptr<Value> MinusNativeFunctionValue::Run(
    EvaluationContext& ev, const CombinationValue* argvalues ) const
{
    CombinationValue::const_iterator it = argvalues->begin();

    if( it == argvalues->end() )
    {
        ArgsChecker::ThrowNotEnoughArgsException( "-", argvalues->size(), 1 );
    }

    int initial_value = 0;

    if( argvalues->size() > 1 )
    {
        const IntegerValue* operand = dynamic_cast<IntegerValue*>( *it );
        if( !operand )
        {
            // Switch into decimal mode and return immediately
            return run_decimal_from_start( it, argvalues );
        }

        initial_value = operand->GetIntValue();

        ++it;
        assert( it != argvalues->end() ); // We have just checked there are more
    }

    auto_ptr<IntegerValue> result( new IntegerValue( initial_value ) );

    for( ; it != argvalues->end(); ++it )
    {
        const IntegerValue* operand = dynamic_cast<IntegerValue*>( *it );
        if( !operand )
        {
            // Switch into decimal mode and return immediately
            return run_decimal_from_int( result.get(), it, argvalues );
        }

        *result -= *operand;
    }

    return auto_ptr<Value>( result.release() );
}


//virtual
MinusNativeFunctionValue* MinusNativeFunctionValue::Clone() const
{
    return new MinusNativeFunctionValue( *this );
}


//virtual
std::string MinusNativeFunctionValue::GetName() const
{
    return StaticName();
}

//static
const std::string& MinusNativeFunctionValue::StaticName()
{
    static const string static_name( "-" );
    return static_name;
}


