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

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "lib/value.h"

class CombinationValue;
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
        CombinationValue* body, const std::string& name,
        const boost::shared_ptr<Environment>& environment );

    /**
     * Note we take ownership of other's execution env even if it didn't own it
     */
    CompoundProcedureValue( const CompoundProcedureValue& other );

    /**
     * Note the new value will own our execution environment even if we don't
     */
    virtual CompoundProcedureValue* Clone() const;

    virtual std::string GetName() const;

    const CombinationValue* GetBody() const;

    boost::shared_ptr<Environment> ExtendEnvironmentWithArgs(
        const CombinationValue* argvalues ) const;

    /**
     * We are being inserted into an environment - if we own it or one of its
     * ancestors, we must stop owning it, since now it will own us, and we
     * don't want a circular reference.
     *
     * We can trust that it won't be deleted while we are still around,
     * because when it's deleted, we will be too.
     */
    void NotifyBeingInsertedInto( const Environment& environment );

private:
    std::auto_ptr<CombinationValue> argnames_;
    std::auto_ptr<CombinationValue> body_;
    std::string name_;

    /**
     * If this procedure is passed around it must carry its environment
     * with it (and the environment must not be deleted).  When this is
     * happening, we hold our environment in this shared pointer, ensuring it
     * is not deleted.
     *
     * However, when this procedure is inserted into its own environment
     * as a symbol, we know the environment will not be deleted because someone
     * else is interested in it, so we don't need a smart pointer to it, and
     * to hold one would cause a circular reference, so in these cases
     * owned_environment_ will be NULL.
     *
     * When owned_environment_ is not NULL, it points to the same env as
     * execution_environment_.
     */
    boost::shared_ptr<Environment> owned_environment_;

    /**
     * This always points to the environment in which this procedure will
     * execute.
     */
    const boost::weak_ptr<Environment> execution_environment_;
};

#endif

