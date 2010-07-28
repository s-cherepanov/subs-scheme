
#ifndef SUBSREPL_H
#define SUBSREPL_H

#include <iosfwd>

class SubsRepl
{
public:
    SubsRepl( bool print_prompt = true );

    int Run( std::istream& in, std::ostream& out, std::ostream& err );

private:
    bool print_prompt_;
};

#endif

