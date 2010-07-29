
#include "stringtreebranch.h"

StringTreeBranch::~StringTreeBranch()
{
    for( iterator it = begin(); it != end(); ++it )
    {
        delete *it;
    }
}

