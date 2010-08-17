
#ifndef CMDLINEARGS_H
#define CMDLINEARGS_H

#include <string>
#include <vector>

class CmdLineArgs
{
public:
    CmdLineArgs( int argc, char * const argv[] );

    bool GetSessionTest() const;

    const std::vector<std::string>& GetNonOptions() const;

private:
    bool session_test_;
    std::vector<std::string> nonoptions_;
};

#endif


