
#ifndef DECIMALVALUE_H
#define DECIMALVALUE_H

#include "value.h"

class DecimalValue : public Value
{
public:
    explicit DecimalValue( double value );

    double GetDoubleValue() const;

    virtual DecimalValue* Clone() const;

private:
    double value_;
};

#endif

