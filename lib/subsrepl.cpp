
#include <iostream>
#include <sstream>

#include "evaluationerror.h"
#include "subsinterpreter.h"

#include "subsrepl.h"

using namespace std;

SubsRepl::SubsRepl( bool print_prompt /*= true */)
: print_prompt_( print_prompt )
{
}

int SubsRepl::Run( istream& in, ostream& out, ostream& err )
{
    static const int bufferlength = 1024;
    char buffer[bufferlength];
    SubsInterpreter interpreter;

    while( in.good() )
    {
        if( print_prompt_ )
        {
            out << "> ";
        }

        // TODO: handle lines longer than bufferlength.
        in.getline( buffer, bufferlength );

        try
        {
            out << interpreter.Interpret( buffer );
        }
        catch( EvaluationError& e )
        {
            err << "Error: " << e.ToString() << endl;
        }

        out << endl;
    }

    return 0;
}

