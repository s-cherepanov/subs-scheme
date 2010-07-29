
#ifndef PROCEDUREVALUE_H
#define PROCEDUREVALUE_H

#include <memory>

#include "value.h"

class Evaluator;

class ProcedureValue : public Value
{
public:
    /**
     * Run this procedure with the supplied (already evaluated)
     * operands.
     *
     * @arg combo is the combination of the operator and operands.
     */
    virtual std::auto_ptr<Value> Run( Evaluator* ev,
        const CombinationValue* combo ) = 0;

    virtual std::string GetName() const = 0;
};

#endif

