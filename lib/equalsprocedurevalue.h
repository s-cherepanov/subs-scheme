
#ifndef EQUALSPROCEDUREVALUE_H
#define EQUALSPROCEDUREVALUE_H

#include "procedurevalue.h"

class Evaluator;
class Environment;

class EqualsProcedureValue : public ProcedureValue
{
public:
    virtual std::auto_ptr<Value> Run( Evaluator* ev,
        const CombinationValue* combo, Environment& environment );

    virtual EqualsProcedureValue* Clone() const;

    virtual std::string GetName() const;
};

#endif

