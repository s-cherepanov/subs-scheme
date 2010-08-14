
#ifndef MINUSPROCEDUREVALUE_H
#define MINUSPROCEDUREVALUE_H

#include <memory>
#include <string>

#include "procedurevalue.h"

class CombinationValue;
class Environment;

class MinusProcedureValue : public ProcedureValue
{
public:
    virtual std::auto_ptr<Value> Run( const CombinationValue* combo,
        const Environment& environment ) const;

    virtual MinusProcedureValue* Clone() const;

    virtual std::string GetName() const;

    static const std::string& StaticName();
};

#endif

