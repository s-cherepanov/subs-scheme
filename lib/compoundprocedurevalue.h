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

#ifndef COMPOUNDPROCEDUREVALUE_H
#define COMPOUNDPROCEDUREVALUE_H

#include <memory>

#include "nativefunctionvalue.h"

class Environment;

/**
 * A procedure defined in Scheme using lambda or the define
 * syntactic sugar.
 *
 * This class takes ownership of the two CombinationValues you pass to it.
 */
 class CompoundProcedureValue : public Value
{
public:
    CompoundProcedureValue( CombinationValue* argnames,
        CombinationValue* body, const std::string& name );

    CompoundProcedureValue( const CompoundProcedureValue& other );

    virtual CompoundProcedureValue* Clone() const;

    virtual std::string GetName() const;

    const CombinationValue* GetBody() const;

    std::auto_ptr<Environment> ExtendEnvironmentWithArgs(
        const CombinationValue* argvalues, const Environment& environment
        ) const;

private:
    std::auto_ptr<CombinationValue> argnames_;
    std::auto_ptr<CombinationValue> body_;
    std::string name_;
};

#endif

