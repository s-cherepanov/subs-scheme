
#ifndef VALUE_H
#define VALUE_H

class Value
{
public:

    virtual ~Value();

    /**
     * Return a deep copy of this value.
     */
    virtual Value* Clone() const = 0;
};


#endif

