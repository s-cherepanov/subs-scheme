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

#ifndef PAIRVALUE_H
#define PAIRVALUE_H

#include <memory>

#include "lib/value/value.h"

class PairValue : public Value
{
public:
    /**
     * Create a pair, taking ownership of the supplied values.
     */
    PairValue( std::auto_ptr<Value> first, std::auto_ptr<Value> second );

    PairValue( const PairValue& other );

    const Value* GetFirst() const;
    const Value* GetSecond() const;

    virtual PairValue* Clone() const;

private:
    std::auto_ptr<Value> first_;
    std::auto_ptr<Value> second_;
};

#endif

