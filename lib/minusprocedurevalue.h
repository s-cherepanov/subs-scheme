
#ifndef MINUSPROCEDUREVALUE_H
#define MINUSPROCEDUREVALUE_H

#include <memory>
#include <string>

#include "procedurevalue.h"

class CombinationValue;
class Evaluator;
class Environment;

class MinusProcedureValue : public ProcedureValue
{
public:
    virtual std::auto_ptr<Value> Run( Evaluator* ev,
        const CombinationValue* combo, Environment& environment );

    virtual MinusProcedureValue* Clone() const;

    virtual std::string GetName() const;

    static const std::string& StaticName();
};

#endif

