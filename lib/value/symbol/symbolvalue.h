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

#ifndef SYMBOLVALUE_H
#define SYMBOLVALUE_H

#include <string>

#include "lib/value/value.h"

class SymbolValue : public Value
{
public:
    virtual const std::string& GetStringValue() const = 0;

    friend bool operator==( const SymbolValue& left,
        const SymbolValue& right );

    friend bool operator!=( const SymbolValue& left,
        const SymbolValue& right );
};

#endif

