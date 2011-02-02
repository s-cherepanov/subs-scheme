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
#include <string>

#include "lib/value/basic/combinationvalue.h"
#include "lib/value/basic/nilvalue.h"
#include "lib/value/basic/pairvalue.h"
#include "lib/value/nativefunction/foreachnativefunctionvalue.h"
#include "lib/argschecker.h"
#include "lib/evaluationcontext.h"
#include "lib/evaluationerror.h"
#include "lib/prettyprinter.h"

using namespace std;

//virtual
std::auto_ptr<Value> ForEachNativeFunctionValue::Run(
    EvaluationContext& ev, const CombinationValue* argvalues ) const
{
    ArgsChecker::CheckExactNumberOfArgs( GetName().c_str(), argvalues, 2 );

    CombinationValue::const_iterator it = argvalues->begin();

    assert( it != argvalues->end() ); // Since we checked there were 2 args

    const Value* fn_value = *it;

    if( !fn_value )
    {
        throw EvaluationError( "Invalid first argument for for-each: nil."
            "  It should be a function." );
    }

    ++it;
    assert( it != argvalues->end() ); // Since we checked there were 2 args

    // TODO: support multiple lists as arguments, picking the nth item
    //       from each list as the arguments to the function being called

    // TODO: if I ever implement list as its own type, this should iterate
    //       over it rather than using GetSecond().
    const Value* list_value = *it;
    while( ! dynamic_cast<const NilValue*>( list_value ) )
    {
        const PairValue* pair = dynamic_cast<const PairValue*>( list_value );
        if( !pair )
        {
            throw EvaluationError( "Invalid argument for for-each: '"
                + PrettyPrinter::Print( *it ) + "' is not a list." );
        }
        const Value* item = pair->GetFirst();

        if( !item )
        {
            // TODO: this should be fine.  Why have I made up a new thing
            //       called NULL, when display etc. could return #t or nil?

            throw EvaluationError(
                "Invalid item in list supplied to for-each (NULL)." );
        }

        CombinationValue combo( 2 );
        combo[0] = fn_value->Clone();
        combo[1] = item->Clone();

        ev.SubEval( &combo );

        list_value = pair->GetSecond();
    }

    return std::auto_ptr<Value>( new NilValue );
}


//virtual
ForEachNativeFunctionValue* ForEachNativeFunctionValue::Clone() const
{
    return new ForEachNativeFunctionValue( *this );
}


//virtual
std::string ForEachNativeFunctionValue::GetName() const
{
    return StaticName();
}

//static
const std::string& ForEachNativeFunctionValue::StaticName()
{
    static const string static_name( "for-each" );
    return static_name;
}


