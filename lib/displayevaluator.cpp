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
        ostringstream ss;
        ss  << "Too many arguments to 'newline'.  There should be none"
            << ", but there were "
            << combo->size() - 1
            << ".";
        throw EvaluationError( ss.str() );
    }

    outstream << std::endl;
}

void write_display( const CombinationValue* combo, std::ostream& outstream )
{
    if( combo->size() != 2 )
    {
        if( combo->size() == 1 )
        {
            throw EvaluationError( "Not enough arguments to 'display'."
                "  There should be 1, but there were none." );
        }
        else
        {
            ostringstream ss;
            ss  << "Too many arguments to 'display'.  There should be 1"
                << ", but there were "
                << combo->size() - 1
                << ".";
            throw EvaluationError( ss.str() );
        }
    }

    const Value* value = (*combo)[1];

    const StringValue* stringvalue = dynamic_cast< const StringValue* >(
        value );
    if( stringvalue )
    {
        outstream << stringvalue->GetStringValue();
    }
    else
    {
        outstream << PrettyPrinter::Print( value );
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
    Environment& environment, const SymbolValue& sym, std::ostream& outstream )
{
    if( is_newline_symbol( sym ) )
    {
        write_newline( combo, outstream );
        return true;
    }
    else if( is_display_symbol( sym ) )
    {
        write_display( combo, outstream );
        return true;
    }

    return false;
}

}
