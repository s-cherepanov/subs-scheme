
#ifndef SYMBOLVALUE_H
#define SYMBOLVALUE_H

#include <string>

#include "value.h"

class SymbolValue : public Value
{
public:
    SymbolValue( std::string symbol );

    const std::string& GetStringValue() const;

    virtual SymbolValue* Clone() const;

private:
    std::string symbol_;
};

#endif

