
#ifndef STRINGTREEBRANCH_H
#define STRINGTREEBRANCH_H

#include <vector>

#include "stringtree.h"

/**
 * A vector of pointers to StringTrees, which are owned by this object.
 */
class StringTreeBranch : public StringTree, public std::vector<StringTree*>
{
public:
    ~StringTreeBranch();
};

#endif

