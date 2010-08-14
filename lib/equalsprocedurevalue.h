
#ifndef EQUALSPROCEDUREVALUE_H
#define EQUALSPROCEDUREVALUE_H

#include "procedurevalue.h"

class Environment;

class EqualsProcedureValue : public ProcedureValue
{
public:
    virtual std::auto_ptr<Value> Run( const CombinationValue* argvalues,
        const Environment& environment ) const;

    virtual EqualsProcedureValue* Clone() const;

    virtual std::string GetName() const;

    static const std::string& StaticName();
};

#endif

