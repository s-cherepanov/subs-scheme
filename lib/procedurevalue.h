
#ifndef PROCEDUREVALUE_H
#define PROCEDUREVALUE_H

#include <memory>
#include <string>

#include "value.h"

class CombinationValue;
class Environment;

class ProcedureValue : public Value
{
public:
    /**
     * Run this procedure with the supplied (already evaluated)
     * operands.
     *
     * @arg combo is the combination of the operator and operands.
     */
    virtual std::auto_ptr<Value> Run( const CombinationValue* argvalues,
        const Environment& environment ) const = 0;

    virtual std::string GetName() const = 0;
};

#endif

