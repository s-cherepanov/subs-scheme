
#ifndef STRINGTREELEAF_H
#define STRINGTREELEAF_H

#include <string>

#include "stringtree.h"

class StringTreeLeaf : public StringTree
{
public:
    StringTreeLeaf( std::string symbolstring );

    std::string str() const;

private:
    std::string symbolstring_;
};

#endif

