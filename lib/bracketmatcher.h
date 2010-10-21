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

#ifndef BRACKETMATCHER_H
#define BRACKETMATCHER_H

#include <iosfwd>

#include "lib/newlineprocessor.h"
#include "lib/tokenlist.h"

class Lexer;

class BracketMatcher
{
public:
    BracketMatcher( Lexer& lexer, std::ostream& out, bool print_continuation );

    ~BracketMatcher();

    /**
     * Read tokens from the stream until the last bracket is matched.
     * Then return the list of tokens found.
     */
    TokenList ReadCombination();

    bool eof() const;

private:
    Lexer& lexer_;
    NewLineProcessor newline_processor_;
    bool print_continuation_;
};

#endif

