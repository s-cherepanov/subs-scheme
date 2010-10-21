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

#ifndef LEXER_H
#define LEXER_H

#include <iostream>

#include "lib/ilexer.h"

class NewLineProcessor;
struct Token;

class Lexer : public ILexer
{
public:
    Lexer( std::istream& instream );

    virtual Token NextToken();

    void SkipWhitespaceToNewline();

    void SetNewLineProcessor( NewLineProcessor* newline_processor );

    bool EndedWithNewLine() const;

    bool eof() const;
private:
    std::istream& instream_;
    char spill_char_;
    bool ended_with_newline_;

    unsigned int column_;
    NewLineProcessor* newline_processor_;
};

#endif

