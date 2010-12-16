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

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "lib/cmdlineargs.h"
#include "lib/replsessiontester.h"
#include "lib/subsinterpreter.h"
#include "lib/subsrepl.h"

using namespace std;


int run_script( const vector<string>& nonoptions )
{
    SubsInterpreter interpreter( cout );
    int ret = 0;
    for( vector<string>::const_iterator it = nonoptions.begin();
        it != nonoptions.end(); ++it )
    {
        if( *it == "-" )
        {
            interpreter.InterpretStream( cin );
        }
        else
        {
            ifstream instream( it->c_str() );
            if( instream.good() )
            {
                interpreter.InterpretStream( instream );
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
        return SubsRepl( cout ).Run( cin, cerr );
    }
    else
    {
        try
        {
            return run_script( nonoptions );
        }
        catch( const exception& e )
        {
            cerr << e.what() << endl;
            return 2;
        }
    }
}

