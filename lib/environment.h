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

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <map>
#include <string>

class Value;

/**
 * A map of symbol names to their values, which owns the values.
 */
class Environment
{
public:
    /**
     * Construct a top-level environment.
     */
    Environment();

    // Note compiler-provided copy constructor exists and performs
    // a shallow copy.

    /**
     * Create an environment by extending another.
     * The second argument is a dummy to differentiate this from
     * a copy constructor.
     */
    Environment( const Environment& parent_to_extend, bool );

    ~Environment();

    /**
     * Add a symbol to this environment.  This object takes ownership
     * of the provided Value object and will delete when it is no
     * longer needed.
     */
    void InsertSymbol( const std::string& name, Value* value );

    const Value* FindSymbol( const std::string& name ) const;

private:

    typedef std::map<std::string, Value*> MapType;
    MapType symbols_;
    const Environment* extended_parent_;
};

#endif

