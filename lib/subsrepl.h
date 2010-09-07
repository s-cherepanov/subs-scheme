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

#ifndef SUBSREPL_H
#define SUBSREPL_H

#include <iosfwd>

#include "subsinterpreter.h"

// TODO: rename this and SubInterpreter to remove "Subs"
// TODO: move this and supporting classes into a subdir
class SubsRepl
{
public:
    enum EResponses
    {
          eNoResponse        = 0
        , ePrintPrompt       = 1
        , ePrintContinuation = 2
        , ePrintWelcome      = 4
    };

    SubsRepl( std::ostream& outstream,
        int responses = ePrintPrompt | ePrintContinuation | ePrintWelcome );

    int Run( std::istream& in, std::ostream& err );

private:
    std::ostream& outstream_;
    SubsInterpreter interpreter_;

    bool print_prompt_;
    bool print_continuation_;
    bool print_welcome_;
};

#endif

