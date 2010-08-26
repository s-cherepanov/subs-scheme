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

#include "evaluator.h"
#include "lexer.h"
#include "parser.h"
#include "prettyprinter.h"
#include "subsinterpreter.h"
#include "tokenlist.h"

using namespace std;

std::string SubsInterpreter::Interpret( const std::string& codestring )
{
    istringstream ss( codestring );
    Lexer lexer( ss );
    Parser parser( lexer );

    return PrettyPrinter::Print( evaluator_.Eval( parser.NextValue().get()
        ).get() );
}

std::string SubsInterpreter::InterpretTokens( TokenList& tokens )
{
    Parser parser( tokens );

    return PrettyPrinter::Print( evaluator_.Eval( parser.NextValue().get()
        ).get() );
}

int SubsInterpreter::InterpretStream( std::istream& instream,
    std::ostream& outstream )
{
    Lexer lexer( instream );
    Parser parser( lexer );

    auto_ptr<Value> value = parser.NextValue();
    while( value.get() )
    {
        string output = PrettyPrinter::Print( evaluator_.Eval( value.get()
            ).get() );

        if( !output.empty() )
        {
            outstream << output << endl;
        }

        value = parser.NextValue();
    }

    // TODO: handle parse errors

    return 0;
}

void SubsInterpreter::SetTracer( Tracer* tracer )
{
    evaluator_.SetTracer( tracer );
}


