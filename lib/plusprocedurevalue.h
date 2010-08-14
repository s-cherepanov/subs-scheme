
#ifndef PLUSPROCEDUREVALUE_H
#define PLUSPROCEDUREVALUE_H

#include <memory>
#include <string>

#include "procedurevalue.h"

class CombinationValue;
class Environment;

class PlusProcedureValue : public ProcedureValue
{
public:
    virtual std::auto_ptr<Value> Run( const CombinationValue* argvalues,
        const Environment& environment ) const;

    virtual PlusProcedureValue* Clone() const;

    virtual std::string GetName() const;

    static const std::string& StaticName();
};

#endif

