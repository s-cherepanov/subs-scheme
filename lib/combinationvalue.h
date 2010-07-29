
#ifndef COMBINATIONVALUE_H
#define COMBINATIONVALUE_H

#include <vector>

#include "value.h"

/**
 * A class representing a combination of Values, which are held by pointer
 * and are owned by this object.
 */
class CombinationValue : public Value, public std::vector<Value*>
{
public:
    virtual ~CombinationValue();
};

#endif

