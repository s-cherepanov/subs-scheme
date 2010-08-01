
#ifndef FALSEVALUE_H
#define FALSEVALUE_H

#include "value.h"

class FalseValue : public Value
{
public:
    virtual FalseValue* Clone() const;
};

#endif

