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

#ifndef DEFINEHELPER_H
#define DEFINEHELPER_H

#include <memory>
#include <string>
#include <boost/shared_ptr.hpp>

#include "lib/value/basic/combinationvalue.h"

class Environment;
class Value;

namespace DefineUtilities
{

std::auto_ptr<Value> define_procedure(
    CombinationValue::const_iterator itarg,
    const CombinationValue::const_iterator& argsend,
    CombinationValue::const_iterator itbody,
    const CombinationValue::const_iterator& bodyend,
    const boost::shared_ptr<Environment>& environment,
    const std::string& name = "" );


/**
 * Insert the supplied value into the supplied environment under the supplied
 * name.  Note that value will be release()d when inserted into environment,
 *
 * If value is a compound procedure, and it owns the environment into which
 * it is being inserted (or one of that environment's parents), stop it
 * from owning it.
 */
void insert_value_into_environment( boost::shared_ptr<Environment>& environment,
    const std::string& name, std::auto_ptr<Value>& value );
}

#endif

