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

#include <iostream>
#include <map>

#include "value.h"

#include "environment.h"

using namespace std;

Environment::Environment()
: extended_parent_( NULL )
{
}

Environment::Environment( const Environment& parent_to_extend, bool )
: extended_parent_( &parent_to_extend )
{
}

Environment::~Environment()
{
    for( MapType::iterator it = symbols_.begin(); it != symbols_.end(); ++it )
    {
        delete it->second;
    }
}

void Environment::InsertSymbol( const std::string& name, Value* value )
{
    MapType::iterator itFind = symbols_.find( name );
    if( itFind != symbols_.end() )
    {
        delete itFind->second;
        itFind->second = value;
    }
    else
    {
        symbols_[name] = value;
    }
}

const Value* Environment::FindSymbol( const std::string& name ) const
{
    MapType::const_iterator itFind = symbols_.find( name );
    if( itFind != symbols_.end() )
    {
        return itFind->second;
    }
    else if( extended_parent_ )
    {
        return extended_parent_->FindSymbol( name );
    }
    else
    {
        return NULL;
    }
}

