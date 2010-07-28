
#include <iostream>

#include "subsinterpreter.h"

#include "subsrepl.h"

using namespace std;

SubsRepl::SubsRepl( bool print_prompt /*= true */)
: print_prompt_( print_prompt )
{
}

int SubsRepl::Run( istream& in, ostream& out, ostream& err )
{
    string inputline;
    SubsInterpreter interpreter;

    while( in.good() )
    {
        if( print_prompt_ )
        {
            out << ">> ";
        }

        // TODO: read until a newline
        in >> inputline;

        out << interpreter.Interpret( inputline ) << endl;
    }

    return 0;
}

