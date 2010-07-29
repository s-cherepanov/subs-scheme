
#ifndef PLUSPROCEDUREVALUE_H
#define PLUSPROCEDUREVALUE_H

#include "procedurevalue.h"

class Evaluator;

class PlusProcedureValue : public ProcedureValue
{
public:
    virtual std::auto_ptr<Value> Run( Evaluator* ev,
        const CombinationValue* combo );

    virtual PlusProcedureValue* Clone() const;

    virtual std::string GetName() const;
};

#endif

