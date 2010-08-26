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

#include <getopt.h>

#include <string>
#include <vector>

#include "cmdlineargs.h"

CmdLineArgs::CmdLineArgs( int argc, char * const argv[] )
: session_test_( false )
{

    int session_test = 0;

    while( true )
    {
        static struct option long_options[] =
        {
              { "session-test", no_argument, &session_test, 1 }
            , { 0, 0, 0, 0 }
        };

        int option_index = 0;
        int c = getopt_long( argc, argv, "", long_options, &option_index );

        if( c == -1 )
            break;
    }

    session_test_ = session_test;

    for( ; optind < argc; ++optind )
    {
        nonoptions_.push_back( argv[optind] );
    }
}

bool CmdLineArgs::GetSessionTest() const
{
    return session_test_;
}

const std::vector<std::string>& CmdLineArgs::GetNonOptions() const
{
    return nonoptions_;
}

