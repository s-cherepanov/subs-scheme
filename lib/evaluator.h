
#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <memory>

#include "environment.h"

class CombinationValue;
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

    const Environment& GetGlobalEnvironment() const;
    Environment& GetGlobalEnvironment();
private:
    Environment global_environment_;
    bool print_intermediates_;
};

#endif

