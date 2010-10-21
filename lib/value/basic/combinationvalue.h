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

#ifndef COMBINATIONVALUE_H
#define COMBINATIONVALUE_H

#include <vector>

#include "lib/value.h"

/**
 * A class representing a combination of Values, which are held by pointer
 * and are owned by this object.
 */
class CombinationValue : public Value, public std::vector<Value*>
{
public:
    CombinationValue();
    CombinationValue( const CombinationValue& other );
    virtual ~CombinationValue();

    virtual CombinationValue* Clone() const;
};

#endif

