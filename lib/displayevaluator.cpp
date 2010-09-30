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

#include <iostream>
#include <sstream>

#include <boost/shared_ptr.hpp>

#include "argschecker.h"
#include "combinationvalue.h"
#include "displayevaluator.h"
#include "environment.h"
#include "evaluationerror.h"
#include "evaluator.h"
#include "prettyprinter.h"
#include "stringvalue.h"
#include "symbolvalue.h"

using namespace std;

namespace
{


bool is_newline_symbol( const SymbolValue& sym )
{
    // TODO: case insensitive?
    return ( sym.GetStringValue() == "newline" );
}


bool is_display_symbol( const SymbolValue& sym )
{
    // TODO: case insensitive?
    return ( sym.GetStringValue() == "display" );
}

void write_newline( const CombinationValue* combo, std::ostream& outstream )
{
    if( combo->size() > 1 )
    {
        ArgsChecker::ThrowWrongNumArgsException( "newline", combo->size() - 1,
            0 );
    }

    outstream << std::endl;
}

void write_display( Evaluator* evaluator, const CombinationValue* combo,
    boost::shared_ptr<Environment>& environment, std::ostream& outstream )
{
    if( combo->size() != 2 )
    {
        ArgsChecker::ThrowWrongNumArgsException( "display", combo->size() - 1,
            1 );
    }

    std::auto_ptr<Value> value = evaluator->EvalInContext(
        (*combo)[1], environment, outstream, true );

    const StringValue* stringvalue = dynamic_cast< const StringValue* >(
        value.get() );
    if( stringvalue )
    {
        outstream << stringvalue->GetStringValue();
    }
    else
    {
        outstream << PrettyPrinter::Print( value.get() );
    }
}

}

namespace DisplayEvaluator
{

/**
 * Check whether the supplied symbol is a display symbol, and if so
 * display as appropriate.
 */
bool ProcessDisplaySymbol( Evaluator* evaluator, const CombinationValue* combo,
    boost::shared_ptr<Environment>& environment, const SymbolValue& sym,
    std::ostream& outstream )
{
    if( is_newline_symbol( sym ) )
    {
        write_newline( combo, outstream );
        return true;
    }
    else if( is_display_symbol( sym ) )
    {
        write_display( evaluator, combo, environment, outstream );
        return true;
    }

    return false;
}

}
