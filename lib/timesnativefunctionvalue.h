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

#ifndef TIMESNATIVEFUNCTIONVALUE_H
#define TIMESNATIVEFUNCTIONVALUE_H

#include <memory>
#include <string>

#include "nativefunctionvalue.h"

class CombinationValue;
class Environment;

class TimesNativeFunctionValue : public NativeFunctionValue
{
public:
    virtual std::auto_ptr<Value> Run( const CombinationValue* argvalues ) const;

    virtual TimesNativeFunctionValue* Clone() const;

    virtual std::string GetName() const;

    static const std::string& StaticName();
};

#endif

