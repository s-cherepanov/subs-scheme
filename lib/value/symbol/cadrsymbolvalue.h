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

#ifndef CADRSYMBOLVALUE_H
#define CADRSYMBOLVALUE_H

#include <iosfwd>
#include <memory>
#include <string>
#include <boost/shared_ptr.hpp>

#include "lib/value/symbol/specialsymbolvalue.h"
#include "lib/value/value.h"
#include "lib/specialsymbolevaluator.h"

class CadrSymbolValue : public SpecialSymbolValue
{
private:
    /**
     * Private: external code uses CreateFromString() to construct
     */
    CadrSymbolValue( const std::string& token_name );

public:
    virtual const std::string& GetStringValue() const;

    static const std::string& StaticValue();

    virtual CadrSymbolValue* Clone() const;

    virtual SpecialSymbolEvaluator::ESymbolType Apply(
        Evaluator* evaluator, const CombinationValue* combo,
        boost::shared_ptr<Environment>& environment,
        std::auto_ptr<Value>& new_value, const Value*& existing_value,
        std::ostream& outstream, bool is_tail_call ) const;

    /**
     * If the supplied string starts with 'c' and ends with 'r' and
     * contains 1, 2, 3 or 4 characters in between which are all either
     * 'a' or 'd', return a new CadrSymbolValue representing this, wrapped
     * in an auto_ptr to Value.
     *
     * Otherwise, return NULL.
     */
     static std::auto_ptr<Value> CreateFromString(
         const std::string& token_name );

private:

    static const unsigned int MAX_AD_LIST_SIZE = 4;
    std::string token_name_;
};

#endif

