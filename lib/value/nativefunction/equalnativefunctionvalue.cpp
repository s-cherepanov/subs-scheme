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
 *  but WITHOUT ANY WARRANTY; without equal the implied warranty of
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
#include <string>
#include <typeinfo>

#include "lib/value/basic/combinationvalue.h"
#include "lib/value/basic/decimalvalue.h"
#include "lib/value/basic/integervalue.h"
#include "lib/value/basic/stringvalue.h"
#include "lib/value/nativefunction/equalnativefunctionvalue.h"
#include "lib/argschecker.h"
#include "lib/evaluationerror.h"
#include "lib/prettyprinter.h"

using namespace std;

namespace
{

bool are_equal( const Value* arg0, const Value* arg1 )
{
    if( typeid( *arg0 ) != typeid( *arg1 ) )
    {
        return false;
    }

    const IntegerValue* int0 = dynamic_cast<const IntegerValue*>( arg0 );
    if( int0 )
    {
        const IntegerValue* int1 = dynamic_cast<const IntegerValue*>( arg1 );
        assert( int1 );
        return ( *int0 == *int1 );
    }

    const DecimalValue* dec0 = dynamic_cast<const DecimalValue*>( arg0 );
    if( dec0 )
    {
        const DecimalValue* dec1 = dynamic_cast<const DecimalValue*>( arg1 );
        assert( dec1 );
        return ( *dec0 == *dec1 );
    }

    const StringValue* str0 = dynamic_cast<const StringValue*>( arg0 );
    if( str0 )
    {
        const StringValue* str1 = dynamic_cast<const StringValue*>( arg1 );
        assert( str1 );
        return ( *str0 == *str1 );
    }

    stringstream ss;
    ss  << "The arguments '"
        << PrettyPrinter::Print( arg0 )
        << "' and '"
        << PrettyPrinter::Print( arg1 )
        << "' are of a (currently) unsupported type for the function "
           "'equals?'.";
    throw EvaluationError( ss.str() );
}

}

//virtual
std::auto_ptr<Value> EqualNativeFunctionValue::Run(
    const CombinationValue* argvalues ) const
{
    ArgsChecker::CheckExactNumberOfArgs( GetName().c_str(), argvalues, 2 );

    return CreateBooleanValue( are_equal( (*argvalues)[0], (*argvalues)[1] ) );
}


//virtual
EqualNativeFunctionValue* EqualNativeFunctionValue::Clone() const
{
    return new EqualNativeFunctionValue( *this );
}


//virtual
std::string EqualNativeFunctionValue::GetName() const
{
    return StaticName();
}

//static
const std::string& EqualNativeFunctionValue::StaticName()
{
    static const string static_name( "equal?" );
    return static_name;
}


