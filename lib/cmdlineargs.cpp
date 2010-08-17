
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

