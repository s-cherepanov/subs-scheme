
#include <string>

#include "stringtreeleaf.h"

using namespace std;

StringTreeLeaf::StringTreeLeaf( std::string symbolstring )
: symbolstring_( symbolstring )
{
}

std::string StringTreeLeaf::str() const
{
    return symbolstring_;
}

