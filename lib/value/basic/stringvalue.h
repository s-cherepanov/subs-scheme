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

#ifndef STRINGVALUE_H
#define STRINGVALUE_H

#include <string>

#include "lib/value/value.h"

class StringValue : public Value
{
public:
    StringValue( const std::string& symbol );

    const std::string& GetStringValue() const;

    virtual StringValue* Clone() const;

    friend bool operator==( const StringValue& left, const StringValue& right );

    friend bool operator!=( const StringValue& left, const StringValue& right );

private:
    std::string value_;
};

#endif

