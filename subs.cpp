
#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>

#include "lib/cmdlineargs.h"
#include "lib/fileinterpreter.h"
#include "lib/subsrepl.h"

using namespace std;



int main( int argc, char * const argv[] )
{
    CmdLineArgs args( argc, argv );

    const vector<string>& nonoptions = args.GetNonOptions();

    if( nonoptions.empty() )
    {
        return SubsRepl().Run( cin, cout, cerr );
    }
    else
    {
        int ret = 0;
        for( vector<string>::const_iterator it = nonoptions.begin();
            it != nonoptions.end(); ++it )
        {
            ret = FileInterpreter( *it ).Interpret( cout );
            if( ret != 0 )
            {
                break;
            }
        }
        return ret;
    }
}

