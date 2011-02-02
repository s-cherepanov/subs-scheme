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

#ifndef ELSESYMBOLVALUE_H
#define ELSESYMBOLVALUE_H

#include <memory>
#include <string>

#include "lib/value/symbol/specialsymbolvalue.h"
#include "lib/value/value.h"
#include "lib/specialsymbolevaluator.h"

class CombinationValue;
class EvaluationContext;

class ElseSymbolValue : public SpecialSymbolValue
{
public:
    virtual const std::string& GetStringValue() const;

    static const std::string& StaticValue();

    virtual ElseSymbolValue* Clone() const;

    virtual SpecialSymbolEvaluator::ESymbolType Apply(
        EvaluationContext& ev, const CombinationValue* combo,
        std::auto_ptr<Value>& new_value, const Value*& existing_value ) const;
};

#endif

