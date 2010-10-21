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

#include "lib/value/basic/combinationvalue.h"
#include "lib/value/basic/stringvalue.h"
#include "lib/argschecker.h"
#include "lib/displayevaluator.h"
#include "lib/environment.h"
#include "lib/evaluator.h"
#include "lib/prettyprinter.h"

using namespace std;

namespace DisplayEvaluator
{


void WriteNewline( const CombinationValue* combo, std::ostream& outstream )
{
    if( combo->size() > 1 )
    {
        ArgsChecker::ThrowWrongNumArgsException( "newline", combo->size() - 1,
            0 );
    }

    outstream << std::endl;
}

void WriteDisplay( Evaluator* evaluator, const CombinationValue* combo,
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
