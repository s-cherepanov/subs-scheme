
#ifndef TIMESNATIVEFUNCTIONVALUE_H
#define TIMESNATIVEFUNCTIONVALUE_H

#include <memory>
#include <string>

#include "nativefunctionvalue.h"

class CombinationValue;
class Environment;

class TimesNativeFunctionValue : public NativeFunctionValue
{
public:
    virtual std::auto_ptr<Value> Run( const CombinationValue* argvalues,
        const Environment& environment ) const;

    virtual TimesNativeFunctionValue* Clone() const;

    virtual std::string GetName() const;

    static const std::string& StaticName();
};

#endif

