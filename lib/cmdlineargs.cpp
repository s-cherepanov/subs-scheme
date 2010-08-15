
#include <string>
#include <vector>

#include "cmdlineargs.h"

CmdLineArgs::CmdLineArgs( int argc, char * const argv[] )
{
    // TODO: options

    for( int i = 1; i < argc; ++i )
    {
        nonoptions_.push_back( argv[i] );
    }
}

const std::vector<std::string>& CmdLineArgs::GetNonOptions()
{
    return nonoptions_;
}

