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
#include "lib/value/nativefunction/maxnativefunctionvalue.h"
#include "lib/argschecker.h"
#include "lib/evaluationerror.h"
#include "lib/prettyprinter.h"
#include "lib/value.h"

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
            if( *decop > *result )
            {
                *result = *decop;
            }
        }
        else
        {
            const IntegerValue* intop = dynamic_cast<IntegerValue*>( *it );

            if( !intop )
            {
                // TODO: remove need for PrettyPrinter scattered everywhere
                throw EvaluationError( "Invalid argument for max: '"
                    + PrettyPrinter::Print( *it )
                    + "' is not an integer or a decimal." );
            }

            if( *intop > *result )
            {
                *result = *intop;
            }
        }
    }
}

std::auto_ptr<Value> run_decimal_from_start(
    CombinationValue::const_iterator it, const CombinationValue* argvalues )
{
    const DecimalValue* decop = dynamic_cast<DecimalValue*>( *it );

    if( !decop )
    {
        throw EvaluationError( "Invalid argument for max: '"
            + PrettyPrinter::Print( *it )
            + "' is not an integer or a decimal." );
    }

    std::auto_ptr<DecimalValue> result( decop->Clone() );

    ++it;

    run_decimal( result, it, argvalues );

    return auto_ptr<Value>( result.release() );
}

std::auto_ptr<Value> run_decimal_from_int( const IntegerValue* intres,
    CombinationValue::const_iterator it, const CombinationValue* argvalues )
{
    std::auto_ptr<DecimalValue> result( new DecimalValue( *intres ) );

    run_decimal( result, it, argvalues );

    return auto_ptr<Value>( result.release() );
}

}

//virtual
std::auto_ptr<Value> MaxNativeFunctionValue::Run(
    const CombinationValue* argvalues ) const
{
    CombinationValue::const_iterator it = argvalues->begin();

    if( it == argvalues->end() )
    {
        ArgsChecker::ThrowNotEnoughArgsException( "max", argvalues->size(), 1 );
    }

    const IntegerValue* operand = dynamic_cast<const IntegerValue*>( *it );

    if( !operand )
    {
        return run_decimal_from_start( it, argvalues );
    }

    // We start with the first operand as the maximum
    auto_ptr<IntegerValue> result( operand->Clone() );

    // Then for each other operand, check whether it's smaller
    ++it;

    for( ; it < argvalues->end(); ++it )
    {
        operand = dynamic_cast<const IntegerValue*>( *it );

        if( !operand )
        {
            // As soon as we find a non-integer, continue the calculation
            // using decimals and return immediately
            return run_decimal_from_int( result.get(), it, argvalues );
        }

        if( *operand > *result )
        {
            *result = *operand;
        }
    }

    return auto_ptr<Value>( result.release() );
}


//virtual
MaxNativeFunctionValue* MaxNativeFunctionValue::Clone() const
{
    return new MaxNativeFunctionValue( *this );
}


//virtual
std::string MaxNativeFunctionValue::GetName() const
{
    return StaticName();
}

//static
const std::string& MaxNativeFunctionValue::StaticName()
{
    static const string static_name( "max" );
    return static_name;
}


