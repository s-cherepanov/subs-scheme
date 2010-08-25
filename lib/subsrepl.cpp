
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

SubsRepl::SubsRepl( bool print_prompt /*= true */)
: print_prompt_( print_prompt )
{
}

int SubsRepl::Run( istream& in, ostream& out, ostream& err )
{
    Lexer lexer( in );
    BracketMatcher matcher( lexer, out );
    SubsInterpreter interpreter;

    try
    {
        while( !matcher.eof() )
        {
            if( print_prompt_ )
            {
                out << "> ";
            }

            lexer.SkipWhitespace();

            TokenList tokens = matcher.ReadCombination();

            string output = interpreter.InterpretTokens( tokens );

            if( !output.empty() )
            {
                out << output << endl;
            }
        }
    }
    catch( EvaluationError& e )
    {
        err << "Error: " << e.ToString() << endl;
    }
    // TODO: catch parse errors too - all under one base class?

    return 0;
}

