
#ifndef PLUSNATIVEFUNCTIONVALUE_H
#define PLUSNATIVEFUNCTIONVALUE_H

#include <memory>
#include <string>

#include "nativefunctionvalue.h"

class CombinationValue;
class Environment;

class PlusNativeFunctionValue : public NativeFunctionValue
{
public:
    virtual std::auto_ptr<Value> Run( const CombinationValue* argvalues,
        const Environment& environment ) const;

    virtual PlusNativeFunctionValue* Clone() const;

    virtual std::string GetName() const;

    static const std::string& StaticName();
};

#endif

