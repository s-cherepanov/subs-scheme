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

#ifndef NEWLINEPROCESSOR_H
#define NEWLINEPROCESSOR_H

#include <iosfwd>
#include <stack>

class NewLineProcessor
{
public:
    NewLineProcessor( std::ostream& out );

    void PushIndent( unsigned int level, unsigned int indent );
    void PopIndent( unsigned int level );

    unsigned int GetIndent() const;

    void NewLine();

    void Reset();

private:
    std::ostream& out_;
    std::stack< std::pair<unsigned int, unsigned int> > indent_stack_;
    unsigned int indent_;
};

#endif

