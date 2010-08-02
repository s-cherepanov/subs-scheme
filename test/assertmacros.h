
#ifndef ASSERTMACROS_H
#define ASSERTMACROS_H

#include <string>
#include <typeinfo>

#define TEST_ASSERT_EQUAL(ARG1,ARG2) \
    assertfns::equal(__FILE__, __LINE__, #ARG1, #ARG2, ARG1, ARG2)

#define TEST_ASSERT_NOT_EQUAL(ARG1,ARG2) \
    assertfns::not_equal(__FILE__, __LINE__, #ARG1, #ARG2, ARG1, ARG2)

#define TEST_ASSERT_NOT_NULL(ARG) \
    assertfns::not_null(__FILE__, __LINE__, #ARG, ARG );

#define TEST_ASSERT_NULL(ARG) \
    assertfns::null(__FILE__, __LINE__, #ARG, ARG );

#define TEST_ASSERT_TRUE(ARG) \
    assertfns::is_true(__FILE__, __LINE__, #ARG, ARG );

#define TEST_ASSERT_FALSE(ARG) \
    assertfns::is_true(__FILE__, __LINE__, #ARG, ARG );

namespace assertfns
{

void equal( std::string file, unsigned int line,
    std::string name1, std::string name2,
    std::string arg1, std::string arg2 );

void equal( std::string file, unsigned int line,
    std::string name1, std::string name2,
    int arg1, int arg2 );

void equal( std::string file, unsigned int line,
    std::string name1, std::string name2,
    const std::type_info& arg1, const std::type_info& arg2 );

void equal( std::string file, unsigned int line,
    std::string name1, std::string name2,
    const void* arg1, const void* arg2 );

void not_equal( std::string file, unsigned int line,
    std::string name1, std::string name2,
    std::string arg1, std::string arg2 );

void not_equal( std::string file, unsigned int line,
    std::string name1, std::string name2,
    int arg1, int arg2 );


void not_null( std::string file, unsigned int line,
    std::string name, void* arg );

void null( std::string file, unsigned int line,
    std::string name, void* arg );


void is_true( std::string file, unsigned int line,
    std::string name, bool arg );

void is_false( std::string file, unsigned int line,
    std::string name, bool arg );

}

#endif

