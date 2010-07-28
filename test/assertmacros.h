
#ifndef ASSERTMACROS_H
#define ASSERTMACROS_H

#include <string>

#define TEST_ASSERT_EQUAL(ARG1,ARG2) \
    assertfns::equal(__FILE__, __LINE__, #ARG1, #ARG2, ARG1, ARG2)

#define TEST_ASSERT_NOT_NULL(ARG) \
    assertfns::not_null(__FILE__, __LINE__, #ARG, ARG );

namespace assertfns
{

void equal( std::string file, unsigned int line,
    std::string name1, std::string name2,
    std::string arg1,  std::string arg2 );

void equal( std::string file, unsigned int line,
    std::string name1, std::string name2,
    int arg1,  int arg2 );

void not_null( std::string file, unsigned int line,
    std::string name,
    void* arg );

}

#endif

