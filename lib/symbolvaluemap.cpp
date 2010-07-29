
#include "value.h"

#include "symbolvaluemap.h"

SymbolValueMap::~SymbolValueMap()
{
    for( iterator it = begin(); it != end(); ++it )
    {
        delete it->second;
    }
}

