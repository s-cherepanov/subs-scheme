
#ifndef STRINGTREELEAF_H
#define STRINGTREELEAF_H

#include <string>

#include "stringtree.h"

class StringTreeLeaf : public StringTree
{
public:
    StringTreeLeaf( const std::string& symbolstring );

    const std::string& str() const;

private:
    std::string symbolstring_;
};

#endif

