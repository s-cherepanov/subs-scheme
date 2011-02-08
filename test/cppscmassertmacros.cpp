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

#include <memory>
#include <string>
#include <sstream>

#include "lib/value/value.h"
#include "lib/evaluator.h"
#include "lib/prettyprinter.h"
#include "test/assertmacros.h"

namespace
{

std::string evaluate_to_string( std::string file, unsigned int line,
    const std::auto_ptr<Value>& val )
{
    std::ostringstream outstream;
    std::auto_ptr<Value> evald = Evaluator().Eval( val.get(), outstream );
    assertfns::equal( file, line, "eval output", "", outstream.str(), "" );
    return PrettyPrinter::Print( evald.get() );

}

}

namespace cppscmassertfns
{

void equal( std::string file, unsigned int line,
    std::string name1, std::string name2,
    const std::auto_ptr<Value>& arg1, const std::auto_ptr<Value>& arg2 )
{
    std::string res1 = evaluate_to_string( file, line, arg1 );
    std::string res2 = evaluate_to_string( file, line, arg2 );

    assertfns::equal( file, line, name1, name2, res1, res2 );
}


void not_equal( std::string file, unsigned int line,
    std::string name1, std::string name2,
    const std::auto_ptr<Value>& arg1, const std::auto_ptr<Value>& arg2 )
{
    std::string res1 = evaluate_to_string( file, line, arg1 );
    std::string res2 = evaluate_to_string( file, line, arg2 );

    assertfns::not_equal( file, line, name1, name2, res1, res2 );
}


}
