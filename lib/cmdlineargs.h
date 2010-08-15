
#ifndef CMDLINEARGS_H
#define CMDLINEARGS_H

#include <string>
#include <vector>

class CmdLineArgs
{
public:
    CmdLineArgs( int argc, char * const argv[] );

    const std::vector<std::string>& GetNonOptions();

private:
    std::vector<std::string> nonoptions_;
};

#endif


