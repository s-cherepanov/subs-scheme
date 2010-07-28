
#include <sstream>
#include <string>

#include "assertionfailed.h"

#include "assertmacros.h"

using namespace std;

namespace assertfns
{

void equal( std::string file, unsigned int line,
    std::string name1, std::string name2,
    std::string arg1,  std::string arg2 )
{
    if( arg1 != arg2 )
    {
        ostringstream ss;
        ss << name1 << " == " << name2
            << " ( \"" << arg1 << "\" != \"" << arg2 << "\" )";
        throw AssertionFailed( file, line, ss.str() );
    }
}

}
