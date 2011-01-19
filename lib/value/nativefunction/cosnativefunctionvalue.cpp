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
#include <cmath>
#include <memory>
#include <sstream>
#include <string>

#include "lib/value/basic/combinationvalue.h"
#include "lib/value/basic/decimalvalue.h"
#include "lib/value/basic/integervalue.h"
#include "lib/value/nativefunction/cosnativefunctionvalue.h"
#include "lib/value/value.h"
#include "lib/argschecker.h"
#include "lib/evaluationerror.h"
#include "lib/prettyprinter.h"

using namespace std;

//virtual
std::auto_ptr<Value> CosNativeFunctionValue::Run(
    EvaluationContext& ev, const CombinationValue* argvalues ) const
{
    ArgsChecker::CheckExactNumberOfArgs( "cos", argvalues, 1 );

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
            throw EvaluationError( "Invalid argument for cos: '"
                    + PrettyPrinter::Print( *it )
                    + "' is not an integer or a decimal." );
        }
        value = operand_int->GetIntValue();
    }

    value = cos( value );

    return auto_ptr<Value>( new DecimalValue( value ) );
}


//virtual
CosNativeFunctionValue* CosNativeFunctionValue::Clone() const
{
    return new CosNativeFunctionValue( *this );
}


//virtual
std::string CosNativeFunctionValue::GetName() const
{
    return StaticName();
}

//static
const std::string& CosNativeFunctionValue::StaticName()
{
    static const string static_name( "cos" );
    return static_name;
}


