
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


void equal( std::string file, unsigned int line,
    std::string name1, std::string name2,
    const std::type_info& arg1, const std::type_info& arg2 )
{
    if( arg1 != arg2 )
    {
        throw_not_equal( file, line, name1, name2,
            "\"" + string( arg1.name() ) + "\"",
            "\"" + string( arg2.name() ) + "\"" );
    }
}

void equal( std::string file, unsigned int line,
    std::string name1, std::string name2,
    const void* arg1, const void* arg2 )
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



void null( std::string file, unsigned int line,
    std::string name,
    void* arg )
{
    if( arg )
    {
        ostringstream ss;
        ss << name << " is null (failed because " << name << " is not null)";
        throw AssertionFailed( file, line, ss.str() );
    }
}


void is_true( std::string file, unsigned int line,
    std::string name,
    bool arg )
{
    if( !arg )
    {
        ostringstream ss;
        ss << name << " is not true (failed because " << name << " is false)";
        throw AssertionFailed( file, line, ss.str() );
    }
}


void is_false( std::string file, unsigned int line,
    std::string name,
    bool arg )
{
    if( arg )
    {
        ostringstream ss;
        ss << name << " is not false (failed because " << name << " is true)";
        throw AssertionFailed( file, line, ss.str() );
    }
}




}
