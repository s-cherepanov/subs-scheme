
#include <string>

#define TEST_ASSERT_EQUAL(ARG1,ARG2) \
    assertfns::equal(__FILE__,__LINE__,#ARG1,#ARG2,ARG1,ARG2)

namespace assertfns
{

void equal( std::string file, unsigned int line,
    std::string name1, std::string name2,
    std::string arg1,  std::string arg2 );

}

