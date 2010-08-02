
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

    friend bool operator==( const IntegerValue& left,
        const IntegerValue& right );

    friend bool operator!=( const IntegerValue& left,
        const IntegerValue& right );

private:
    int value_;
};

#endif

