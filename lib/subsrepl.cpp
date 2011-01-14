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

#include <exception>
#include <iostream>
#include <sstream>

#include "lib/bracketmatcher.h"
#include "lib/evaluationerror.h"
#include "lib/lexer.h"
#include "lib/prettyprinter.h"
#include "lib/subsinterpreter.h"
#include "lib/subsrepl.h"
#include "lib/tokenlist.h"

using namespace std;

namespace
{

void write_welcome( ostream& out )
{
    out << "Subs Scheme Interpreter, Copyright (C) 2010 Andy Balaam" << endl
        << "Subs comes with ABSOLUTELY NO WARRANTY"
            "."
            // TODO: support warranty "; for details type 'warranty'."
            << endl
        << "This is free software, and you are welcome to redistribute it"
            << endl
        << "under certain conditions"
            "."
            // TODO: support copying "; type `copying' for details."
            << endl;
}

}

SubsRepl::SubsRepl( std::ostream& outstream,
    int responses /*= ePrintPrompt | ePrintContinuation | ePrintWelcome*/ )
: outstream_( outstream )
, interpreter_( outstream )
, print_prompt_(       responses & ePrintPrompt )
, print_continuation_( responses & ePrintContinuation )
, print_welcome_(      responses & ePrintWelcome )
{
}

int SubsRepl::Run( std::istream& instream, std::ostream& errstream )
{
    if( print_welcome_ )
    {
        write_welcome( outstream_ );
    }

    Lexer lexer( instream );
    BracketMatcher matcher( lexer, outstream_, print_continuation_ );

    while( !matcher.eof() )
    {
        try
        {
            if( print_prompt_ )
            {
                outstream_ << "> ";
            }

            TokenList tokens = matcher.ReadCombination();

            interpreter_.InterpretTokens( tokens );

            lexer.SkipWhitespaceToNewline();
        }
        catch( exception& e )
        {
            errstream << "Error: " << e.what() << endl;
        }
    }

    return 0;
}

