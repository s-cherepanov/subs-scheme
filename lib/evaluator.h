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

#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <iosfwd>
#include <memory>

#include <boost/shared_ptr.hpp>

#include "lib/environment.h"
#include "lib/nulltracer.h"

class CombinationValue;
class Tracer;
class Value;

class Evaluator
{
public:
    Evaluator();

    /**
     * Evaluate the expression supplied, and return the value of its result.
     * For constant expressions, we return the value with which we were
     * supplied.
     */
    std::auto_ptr<Value> Eval( const Value* value, std::ostream& outstream );

    std::auto_ptr<Value> EvalInContext( const Value* value,
        boost::shared_ptr<Environment>& environment,
        std::ostream& outstream, bool is_tail_call );

    void SetTracer( Tracer* tracer );
    Tracer* GetTracer();

private:

    boost::shared_ptr<Environment> global_environment_;

    Tracer* tracer_;
    NullTracer null_tracer_;
};

#endif

