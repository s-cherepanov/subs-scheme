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
#include <sstream>

#include "lib/value/basic/combinationvalue.h"
#include "lib/value/basic/integervalue.h"
#include "lib/value/nativefunction/remaindernativefunctionvalue.h"
#include "lib/argschecker.h"
#include "lib/evaluationerror.h"
#include "lib/prettyprinter.h"
#include "lib/value.h"

using namespace std;

//virtual
std::auto_ptr<Value> RemainderNativeFunctionValue::Run(
    const CombinationValue* argvalues ) const
{
    ArgsChecker::CheckExactNumberOfArgs( "remainder", argvalues, 2 );

    CombinationValue::const_iterator it = argvalues->begin();
    const IntegerValue* dividend = dynamic_cast<IntegerValue*>( *it );
    if( !dividend )
    {
        throw EvaluationError( "Invalid argument for remainder: '"
            + PrettyPrinter::Print( *it ) + "' is not an integer." );
    }

    ++it;
    assert( it != argvalues->end() ); // We checked above there were 2
    const IntegerValue* divisor = dynamic_cast<IntegerValue*>( *it );
    if( !divisor )
    {
        throw EvaluationError( "Invalid argument for remainder: '"
            + PrettyPrinter::Print( *it ) + "' is not an integer." );
    }

    return auto_ptr<Value> ( new IntegerValue(
        dividend->GetIntValue() % divisor->GetIntValue() ) );
}


//virtual
RemainderNativeFunctionValue* RemainderNativeFunctionValue::Clone() const
{
    return new RemainderNativeFunctionValue( *this );
}


//virtual
std::string RemainderNativeFunctionValue::GetName() const
{
    return StaticName();
}

//static
const std::string& RemainderNativeFunctionValue::StaticName()
{
    static const string static_name( "remainder" );
    return static_name;
}


