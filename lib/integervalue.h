
#ifndef INTEGERVALUE_H
#define INTEGERVALUE_H

#include "value.h"

class IntegerValue : public Value
{
public:
    explicit IntegerValue( int value );

    IntegerValue& operator+=( const IntegerValue& other );

    int GetIntValue() const;

    virtual IntegerValue* Clone() const;

private:
    int value_;
};

#endif

