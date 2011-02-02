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
#include <fstream>
#include <iostream>
#include <memory>

#include "lib/value/basic/combinationvalue.h"
#include "lib/value/basic/stringvalue.h"
#include "lib/value/nativefunction/loadnativefunctionvalue.h"
#include "lib/value/value.h"
#include "lib/argschecker.h"
#include "lib/evaluationcontext.h"
#include "lib/evaluationerror.h"
#include "lib/lexer.h"
#include "lib/parser.h"
#include "lib/prettyprinter.h"
#include "lib/specialsymbolevaluator.h"

// TODO: is it dirty to depend on lexer and parser?

using namespace std;

namespace
{

void interpret_stream( istream& instream, EvaluationContext& ev )
{
    // TODO: share code with SubsInterpreter?

    Lexer lexer( instream );
    Parser parser( lexer );

    auto_ptr<Value> value = parser.NextValue();
    while( value.get() )
    {
        string output = PrettyPrinter::Print( ev.SubEval( value.get()
            ).get() );

        value = parser.NextValue();
    }
}

void load_file( const string& filename, EvaluationContext& ev )
{
    ifstream instream( filename.c_str() );
    if( instream.good() )
    {
        try
        {
            interpret_stream( instream, ev );
        }
        catch( const exception& e )
        {
            throw EvaluationError( "Error during 'load' of file '" + filename +
                "': " + e.what() );
        }
    }
    else
    {
        throw EvaluationError( "Error during 'load' - the file '"
            + filename + "' does not exist." );
    }
}


}


//virtual
std::auto_ptr<Value> LoadNativeFunctionValue::Run(
    EvaluationContext& ev, const CombinationValue* argvalues ) const
{
    ArgsChecker::CheckExactNumberOfArgs( "load", argvalues, 1 );

    CombinationValue::const_iterator it = argvalues->begin();

    assert( it != argvalues->end() ); // We have checked there is 1 arg

    const StringValue* stringvalue = dynamic_cast<const StringValue*>( *it );

    if( !stringvalue )
    {
        throw EvaluationError( "The argument to load must be a string filename "
            "- '" + PrettyPrinter::Print( *it ) + "' is not a string." );
    }

    load_file( stringvalue->GetStringValue(), ev );

    return std::auto_ptr<Value>( NULL );
}


//virtual
LoadNativeFunctionValue* LoadNativeFunctionValue::Clone() const
{
    return new LoadNativeFunctionValue( *this );
}


//virtual
std::string LoadNativeFunctionValue::GetName() const
{
    return StaticName();
}

//static
const std::string& LoadNativeFunctionValue::StaticName()
{
    static const string static_name( "load" );
    return static_name;
}


