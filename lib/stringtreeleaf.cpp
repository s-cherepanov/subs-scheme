
#include <string>

#include "stringtreeleaf.h"

using namespace std;

StringTreeLeaf::StringTreeLeaf( const std::string& symbolstring )
: symbolstring_( symbolstring )
{
}

const std::string& StringTreeLeaf::str() const
{
    return symbolstring_;
}

