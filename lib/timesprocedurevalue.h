
#ifndef TIMESPROCEDUREVALUE_H
#define TIMESPROCEDUREVALUE_H

#include <memory>
#include <string>

#include "procedurevalue.h"

class CombinationValue;
class Evaluator;
class Environment;

class TimesProcedureValue : public ProcedureValue
{
public:
    virtual std::auto_ptr<Value> Run( Evaluator* ev,
        const CombinationValue* combo, Environment& environment );

    virtual TimesProcedureValue* Clone() const;

    virtual std::string GetName() const;

    static const std::string& StaticName();
};

#endif

