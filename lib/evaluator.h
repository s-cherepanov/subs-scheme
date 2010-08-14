
#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <memory>

#include "environment.h"
#include "nulltracer.h"

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
    std::auto_ptr<Value> Eval( const Value* value );

    void SetTracer( Tracer* tracer );
    Tracer* GetTracer();

private:

    Environment global_environment_;

    Tracer* tracer_;
    NullTracer null_tracer_;
};

#endif

