
#ifndef INTEGERVALUE_H
#define INTEGERVALUE_H

#include "value.h"

class IntegerValue : public Value
{
public:
    explicit IntegerValue( int value );

    int GetIntValue() const;

private:
    int value_;
};

#endif

