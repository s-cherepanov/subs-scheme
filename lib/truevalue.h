
#ifndef TRUEVALUE_H
#define TRUEVALUE_H

#include "value.h"

class TrueValue : public Value
{
public:
    virtual TrueValue* Clone() const;
};

#endif

