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
#include "lib/value/nativefunction/timesnativefunctionvalue.h"
#include "lib/evaluationerror.h"
#include "lib/prettyprinter.h"
#include "lib/value.h"

using namespace std;

namespace
{

std::auto_ptr<Value> run_decimal( const IntegerValue* intres,
    CombinationValue::const_iterator it, const CombinationValue* argvalues )
{
    auto_ptr<DecimalValue> result( new DecimalValue( *intres ) );

    for( ; it != argvalues->end(); ++it )
    {
        const DecimalValue* decop = dynamic_cast<DecimalValue*>( *it );
        if( decop )
        {
            *result *= *decop;
        }
        else
        {
            const IntegerValue* intop = dynamic_cast<IntegerValue*>( *it );
            if( !intop )
            {
                throw EvaluationError( "Invalid argument for *: '"
                    + PrettyPrinter::Print( *it )
                    + "' is not an integer or a decimal." );
            }
            else
            {
                *result *= *intop;
            }
        }
    }

    return auto_ptr<Value>( result.release() );
}

}

//virtual
std::auto_ptr<Value> TimesNativeFunctionValue::Run(
    const CombinationValue* argvalues ) const
{
    auto_ptr<IntegerValue> result( new IntegerValue( 1 ) );

    CombinationValue::const_iterator it = argvalues->begin();

    for( ; it != argvalues->end(); ++it )
    {
        const IntegerValue* operand = dynamic_cast<IntegerValue*>( *it );
        if( !operand )
        {
            return run_decimal( result.get(), it, argvalues );
            
        }

        *result *= *operand;
    }

    return auto_ptr<Value>( result.release() );
}


//virtual
TimesNativeFunctionValue* TimesNativeFunctionValue::Clone() const
{
    return new TimesNativeFunctionValue( *this );
}


//virtual
std::string TimesNativeFunctionValue::GetName() const
{
    return StaticName();
}

//static
const std::string& TimesNativeFunctionValue::StaticName()
{
    static const string static_name( "*" );
    return static_name;
}


