
#include "combinationvalue.h"

CombinationValue::~CombinationValue()
{
    for( iterator it = begin(); it != end(); ++it )
    {
        delete *it;
    }
}

