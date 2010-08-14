
#ifndef MINUSNATIVEFUNCTIONVALUE_H
#define MINUSNATIVEFUNCTIONVALUE_H

#include <memory>
#include <string>

#include "nativefunctionvalue.h"

class CombinationValue;
class Environment;

class MinusNativeFunctionValue : public NativeFunctionValue
{
public:
    virtual std::auto_ptr<Value> Run( const CombinationValue* combo,
        const Environment& environment ) const;

    virtual MinusNativeFunctionValue* Clone() const;

    virtual std::string GetName() const;

    static const std::string& StaticName();
};

#endif

