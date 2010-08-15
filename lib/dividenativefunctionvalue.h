
#ifndef DIVIDENATIVEFUNCTIONVALUE_H
#define DIVIDENATIVEFUNCTIONVALUE_H

#include <memory>
#include <string>

#include "nativefunctionvalue.h"

class CombinationValue;
class Environment;

class DivideNativeFunctionValue : public NativeFunctionValue
{
public:
    virtual std::auto_ptr<Value> Run( const CombinationValue* argvalues,
        const Environment& environment ) const;

    virtual DivideNativeFunctionValue* Clone() const;

    virtual std::string GetName() const;

    static const std::string& StaticName();
};

#endif

