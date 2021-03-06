/*
 *  Subs Scheme Interpreter
 *  Copyright (C) 2010, 2011 Andy Balaam
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

#include <sstream>
#include <string>

#include "test/assertionfailed.h"
#include "test/assertmacros.h"

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

namespace utils
{

std::string get_cmd_plus_args( const std::string& cmd, unsigned int num_args )
{
    std::string ret = "(";
    ret += cmd;
    for( unsigned int i = 0; i < num_args; ++i )
    {
        ret += " ";
        ret += "1";
    }
    ret += ")";
    return ret;
}

}


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
    const void* arg )
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
    const void* arg )
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


void can_find( std::string file, unsigned int line,
    std::string lookinname, std::string lookforname,
    std::string lookin, std::string lookfor )
{
    if( lookin.find( lookfor ) == string::npos )
    {
        ostringstream ss;
        ss << "Cannot find '" << lookfor << "' in '" << lookin << "'.";
        throw AssertionFailed( file, line, ss.str() );
    }
}




}
