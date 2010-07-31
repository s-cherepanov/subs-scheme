
#include "value.h"

#include "environment.h"

Environment::~Environment()
{
    for( iterator it = begin(); it != end(); ++it )
    {
        delete it->second;
    }
}

