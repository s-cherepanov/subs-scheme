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
#include <memory>
#include <sstream>
#include <string>

#include "lib/evaluator.h"
#include "lib/lexer.h"
#include "lib/parser.h"
#include "lib/prettyprinter.h"
#include "lib/subsinterpreter.h"
#include "lib/tokenlist.h"
#include "lib/value/value.h"
#include "scmlib/scmlib.h"

using namespace std;


namespace
{

void interpret_values( Parser& parser, Evaluator& evaluator,
    ostream& outstream, bool print_values, bool print_newline )
{
    auto_ptr<Value> value = parser.NextValue();
    while( value.get() )
    {
        string output = PrettyPrinter::Print( evaluator.Eval( value.get(),
            outstream ).get() );

        if( print_values && !output.empty() )
        {
            outstream << output;
            if( print_newline )
            {
                outstream << endl;
            }
        }

        value = parser.NextValue();
    }
}

}


SubsInterpreter::SubsInterpreter( std::ostream& outstream /*= std::cout*/ )
: outstream_( outstream )
{
    scmlib_load( *this );
}

std::string SubsInterpreter::Interpret( const std::string& codestring )
{
    istringstream in( codestring );
    ostringstream out;

    Lexer lexer( in );
    Parser parser( lexer );

    interpret_values( parser, evaluator_, out, true, false );

    return out.str();
}

void SubsInterpreter::InterpretTokens( TokenList& tokens )
{
    Parser parser( tokens );

    interpret_values( parser, evaluator_, outstream_, true, true );
}

void SubsInterpreter::InterpretStream( std::istream& instream )
{
    Lexer lexer( instream );
    Parser parser( lexer );

    interpret_values( parser, evaluator_, outstream_, false, true );

    // Any errors result in exceptions being thrown inside interpret_values
}

void SubsInterpreter::SetTracer( Tracer* tracer )
{
    evaluator_.SetTracer( tracer );
}


