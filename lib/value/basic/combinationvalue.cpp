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

#include "lib/value/basic/combinationvalue.h"

CombinationValue::CombinationValue()
{
}

CombinationValue::CombinationValue( std::vector<Value*>::size_type size )
: std::vector<Value*>( size )
{
}

CombinationValue::CombinationValue( const CombinationValue& other )
{
    reserve( other.size() );

    for( CombinationValue::const_iterator it = other.begin();
        it != other.end(); ++it )
    {
        push_back( (*it)->Clone() );
    }
}

CombinationValue::~CombinationValue()
{
    for( iterator it = begin(); it != end(); ++it )
    {
        delete *it;
    }
}

//virtual
CombinationValue* CombinationValue::Clone() const
{
    return new CombinationValue( *this );
}

