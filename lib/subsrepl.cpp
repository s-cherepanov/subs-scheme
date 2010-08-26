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

#include "bracketmatcher.h"
#include "evaluationerror.h"
#include "lexer.h"
#include "prettyprinter.h"
#include "subsinterpreter.h"
#include "subsrepl.h"
#include "tokenlist.h"

using namespace std;

namespace
{

void write_welcome( ostream& out )
{
    out << "Subs Scheme Interpreter, Copyright (C) 2010 Andy Balaam" << endl
        << "Subs comes with ABSOLUTELY NO WARRANTY; for details type "
            "'warranty'." << endl
        << "This is free software, and you are welcome to redistribute it"
            << endl
        << "under certain conditions; type `copying' for details." << endl;
}

}

SubsRepl::SubsRepl( bool print_prompt /*= true */,
    bool print_welcome /*= true*/ )
: print_prompt_( print_prompt )
, print_welcome_( print_welcome )
{
}

int SubsRepl::Run( istream& in, ostream& out, ostream& err )
{
    if( print_welcome_ )
    {
        write_welcome( out );
    }

    Lexer lexer( in );
    BracketMatcher matcher( lexer, out, print_prompt_ );
    SubsInterpreter interpreter;

    try
    {
        while( !matcher.eof() )
        {
            if( print_prompt_ )
            {
                out << "> ";
            }

            TokenList tokens = matcher.ReadCombination();

            string output = interpreter.InterpretTokens( tokens );

            if( !output.empty() )
            {
                out << output << endl;
            }
            lexer.SkipWhitespaceToNewline();
        }
    }
    catch( EvaluationError& e )
    {
        err << "Error: " << e.ToString() << endl;
    }
    // TODO: catch parse errors too - all under one base class?

    return 0;
}

