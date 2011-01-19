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

#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "lib/value/basic/combinationvalue.h"
#include "lib/value/basic/stringvalue.h"
#include "lib/value/symbol/loadsymbolvalue.h"
#include "lib/value/value.h"
#include "lib/argschecker.h"
#include "lib/environment.h"
#include "lib/evaluationcontext.h"
#include "lib/evaluationerror.h"
#include "lib/evaluator.h"
#include "lib/lexer.h"
#include "lib/parser.h"
#include "lib/prettyprinter.h"
#include "lib/specialsymbolevaluator.h"

// TODO: is it dirty to depend on lexer and parser?

using namespace std;

namespace
{

void interpret_stream( istream& instream, Evaluator* evaluator,
    boost::shared_ptr<Environment>& environment, ostream& outstream )
{
    // TODO: share code with SubsInterpreter?

    Lexer lexer( instream );
    Parser parser( lexer );

    auto_ptr<Value> value = parser.NextValue();
    while( value.get() )
    {
        string output = PrettyPrinter::Print( evaluator->EvalInContext(
            value.get(), environment, outstream, true ).get() );

        value = parser.NextValue();
    }
}

void load_file( const string& filename, Evaluator* evaluator,
    boost::shared_ptr<Environment>& environment, ostream& outstream )
{
    ifstream instream( filename.c_str() );
    if( instream.good() )
    {
        try
        {
            interpret_stream( instream, evaluator, environment, outstream );
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
const std::string& LoadSymbolValue::GetStringValue() const
{
    return StaticValue();
}

//static
const std::string& LoadSymbolValue::StaticValue()
{
    static const std::string ret( "load" );
    return ret;
}

//virtual
LoadSymbolValue* LoadSymbolValue::Clone() const
{
    return new LoadSymbolValue( *this );
}

//virtual
SpecialSymbolEvaluator::ESymbolType LoadSymbolValue::Apply(
    EvaluationContext& ev, const CombinationValue* combo,
    std::auto_ptr<Value>& new_value, const Value*& existing_value ) const
{
    if( combo->size() != 2 )
    {
        ArgsChecker::ThrowWrongNumArgsException( "load", combo->size() - 1, 1 );
    }

    CombinationValue::const_iterator it = combo->begin();

    assert( it != combo->end() ); // "load" (ignore)

    ++it;

    assert( it != combo->end() ); // the filename

    std::auto_ptr<Value> evald_value = ev.evaluator_->EvalInContext( *it,
         ev.environment_, ev.outstream_, false );

    const Value* value = evald_value.get();
    const StringValue* stringvalue = dynamic_cast<const StringValue*>( value );

    if( !stringvalue )
    {
        throw EvaluationError( "The argument to load must be a string filename "
            "- '" + PrettyPrinter::Print( value ) + "' is not a string." );
    }

    load_file( stringvalue->GetStringValue(), ev.evaluator_, ev.environment_,
        ev.outstream_ );

    existing_value = NULL;
    return SpecialSymbolEvaluator::eEvaluateExistingSymbol;
}


