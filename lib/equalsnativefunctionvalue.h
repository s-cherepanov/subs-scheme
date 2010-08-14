
#ifndef EQUALSNATIVEFUNCTIONVALUE_H
#define EQUALSNATIVEFUNCTIONVALUE_H

#include "nativefunctionvalue.h"

class Environment;

class EqualsNativeFunctionValue : public NativeFunctionValue
{
public:
    virtual std::auto_ptr<Value> Run( const CombinationValue* argvalues,
        const Environment& environment ) const;

    virtual EqualsNativeFunctionValue* Clone() const;

    virtual std::string GetName() const;

    static const std::string& StaticName();
};

#endif

