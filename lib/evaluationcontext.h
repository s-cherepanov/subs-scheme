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

#ifndef EVALUATION_CONTEXT_H
#define EVALUATION_CONTEXT_H

#include <iosfwd>
#include <memory>

#include <boost/shared_ptr.hpp>

#include "lib/value/value.h"

class Evaluator;
class Environment;

class EvaluationContext
{
public:
    EvaluationContext( Evaluator& evaluator,
        boost::shared_ptr<Environment>& environment, std::ostream& outstream,
        bool is_tail_call );

    std::auto_ptr<Value> SubEval( const Value* value );

    boost::shared_ptr<Environment>& GetEnvironment() { return environment_; }

    std::ostream& GetOutStream() { return outstream_; }

    bool GetIsTailCall() { return is_tail_call_; }

private:
    Evaluator& evaluator_;
    boost::shared_ptr<Environment>& environment_;
    std::ostream& outstream_;
    bool is_tail_call_;
};

#endif
