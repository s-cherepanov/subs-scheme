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

#include "combinationvalue.h"
#include "displayevaluator.h"
#include "environment.h"
#include "evaluator.h"
#include "symbolvalue.h"

namespace
{


bool is_newline_symbol( const SymbolValue& sym )
{
    // TODO: case insensitive?
    return ( sym.GetStringValue() == "newline" );
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
        outstream << std::endl;
        return true;
    }

    return false;
}

}