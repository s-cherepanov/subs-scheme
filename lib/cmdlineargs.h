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


