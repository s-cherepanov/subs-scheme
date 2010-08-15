
#ifndef DECIMALVALUE_H
#define DECIMALVALUE_H

#include <memory>

#include "value.h"

class IntegerValue;

class DecimalValue : public Value
{
public:
    explicit DecimalValue( double value );

    double GetDoubleValue() const;

    virtual DecimalValue* Clone() const;

    friend std::auto_ptr<Value> operator/( const DecimalValue& left,
        const IntegerValue& right );

    friend std::auto_ptr<Value> operator/( const IntegerValue& left,
        const DecimalValue& right );

    friend std::auto_ptr<Value> operator/( const DecimalValue& left,
        const DecimalValue& right );

private:
    double value_;
};

#endif

