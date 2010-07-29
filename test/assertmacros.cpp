
#include <sstream>
#include <string>

#include "assertionfailed.h"

#include "assertmacros.h"

using namespace std;

namespace
{

void throw_not_equal( std::string file, unsigned int line,
    std::string name1, std::string name2,
    std::string arg1,  std::string arg2 )
{
    ostringstream ss;
    ss << name1 << " == " << name2
        << " (failed because " << arg1 << " != " << arg2 << ")";
    throw AssertionFailed( file, line, ss.str() );
}


void throw_equal( std::string file, unsigned int line,
    std::string name1, std::string name2,
    std::string arg1,  std::string arg2 )
{
    ostringstream ss;
    ss << name1 << " != " << name2
        << " (failed because " << arg1 << " == " << arg2 << ")";
    throw AssertionFailed( file, line, ss.str() );
}

}

namespace assertfns
{

void equal( std::string file, unsigned int line,
    std::string name1, std::string name2,
    std::string arg1,  std::string arg2 )
{
    if( arg1 != arg2 )
    {
        throw_not_equal( file, line, name1, name2,
            "\"" + arg1 + "\"", "\"" + arg2 + "\"" );
    }
}

void equal( std::string file, unsigned int line,
    std::string name1, std::string name2,
    int arg1, int arg2 )
{
    if( arg1 != arg2 )
    {
        ostringstream ss1;
        ostringstream ss2;
        ss1 << arg1;
        ss2 << arg2;
        throw_not_equal( file, line, name1, name2, ss1.str(), ss2.str() );
    }
}

void not_equal( std::string file, unsigned int line,
    std::string name1, std::string name2,
    std::string arg1,  std::string arg2 )
{
    if( arg1 == arg2 )
    {
        throw_equal( file, line, name1, name2,
            "\"" + arg1 + "\"", "\"" + arg2 + "\"" );
    }
}

void not_equal( std::string file, unsigned int line,
    std::string name1, std::string name2,
    int arg1, int arg2 )
{
    if( arg1 == arg2 )
    {
        ostringstream ss1;
        ostringstream ss2;
        ss1 << arg1;
        ss2 << arg2;
        throw_equal( file, line, name1, name2, ss1.str(), ss2.str() );
    }
}


void not_null( std::string file, unsigned int line,
    std::string name,
    void* arg )
{
    if( !arg )
    {
        ostringstream ss;
        ss << name << " is not null (failed because " << name << " is null)";
        throw AssertionFailed( file, line, ss.str() );
    }
}


}
