
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "lib/cmdlineargs.h"
#include "lib/replsessiontester.h"
#include "lib/subsinterpreter.h"
#include "lib/subsrepl.h"

using namespace std;



int main( int argc, char * const argv[] )
{
    CmdLineArgs args( argc, argv );

    const vector<string>& nonoptions = args.GetNonOptions();

    if( args.GetSessionTest() )
    {
        return ReplSessionTester().Run( nonoptions );
    }
    else if( nonoptions.empty() )
    {
        return SubsRepl().Run( cin, cout, cerr );
    }
    else
    {
        SubsInterpreter interpreter;
        int ret = 0;
        for( vector<string>::const_iterator it = nonoptions.begin();
            it != nonoptions.end(); ++it )
        {
            if( *it == "-" )
            {
                ret = interpreter.InterpretStream( cin, cout );
            }
            else
            {
                ifstream instream( it->c_str() );
                if( instream.good() )
                {
                    ret = interpreter.InterpretStream( instream, cout );
                }
                else
                {
                    cerr << "subs: Error: unable to open file '"
                        << *it << "'." << endl;
                    ret = 1;
                }
            }

            if( ret != 0 )
            {
                break;
            }
        }
        return ret;
    }
}

