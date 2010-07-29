
#ifndef PLUSPROCEDUREVALUE_H
#define PLUSPROCEDUREVALUE_H

#include "procedurevalue.h"

class PlusProcedureValue : public ProcedureValue
{
public:
    virtual std::auto_ptr<Value> Run( const CombinationValue* combo );

    virtual PlusProcedureValue* Clone() const;

    virtual std::string GetName() const;
};

#endif

