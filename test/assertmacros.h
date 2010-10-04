/*
 *  Subs Scheme Interpreter
 *  Copyright (C) 2010 Andy Balaam
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**/

#ifndef ASSERTMACROS_H
#define ASSERTMACROS_H

#include <iostream>
#include <string>
#include <typeinfo>

#include "lib/evaluationerror.h"
#include "lib/subsinterpreter.h"

#define RUN_TEST(TESTNAME) \
    { \
        try \
        { \
            TESTNAME(); \
        } \
        catch( EvaluationError& e ) \
        { \
            std::cerr << "Error running test '" #TESTNAME "' in suite '" SUITENAME "':" << std::endl; \
            throw; \
        } \
    }

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
    assertfns::is_false(__FILE__, __LINE__, #ARG, ARG );

#define TEST_ASSERT_CAN_FIND(LOOKIN,LOOKFOR) \
    assertfns::can_find(__FILE__, __LINE__, #LOOKIN, #LOOKFOR, LOOKIN, LOOKFOR);

#define TEST_ASSERT_THROWS_BEGIN \
    { \
        bool exception_thrown = false; \
        try

#define TEST_ASSERT_THROWS_END(EXPECTED) \
        catch( EvaluationError& e ) \
        { \
            exception_thrown = true; \
            TEST_ASSERT_CAN_FIND(e.ToString(),EXPECTED); \
        } \
        TEST_ASSERT_TRUE( exception_thrown ); \
    }

#define TEST_ASSERT_THROWS_END2(EXPECTED1,EXPECTED2) \
        catch( EvaluationError& e ) \
        { \
            exception_thrown = true; \
            TEST_ASSERT_CAN_FIND(e.ToString(),EXPECTED1); \
            TEST_ASSERT_CAN_FIND(e.ToString(),EXPECTED2); \
        } \
        TEST_ASSERT_TRUE( exception_thrown ); \
    }

#define TEST_ASSERT_TAKES_FIXED_NUMBER_OF_ARGS(CMD, NUM) \
    { \
        SubsInterpreter interpreter; \
        \
        TEST_ASSERT_THROWS_BEGIN \
        { \
            interpreter.Interpret( assertfns::utils::get_cmd_plus_args( CMD, \
                NUM - 1 ) ); \
        } \
        TEST_ASSERT_THROWS_END("Not enough") \
        \
        TEST_ASSERT_THROWS_BEGIN \
        { \
            interpreter.Interpret( assertfns::utils::get_cmd_plus_args( CMD, \
                NUM + 1 ) ); \
        } \
        TEST_ASSERT_THROWS_END("Too many") \
    }

namespace assertfns
{

namespace utils
{

std::string get_cmd_plus_args( const std::string& cmd, unsigned int num_args );

}

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
    std::string name, const void* arg );

void null( std::string file, unsigned int line,
    std::string name, const void* arg );


void is_true( std::string file, unsigned int line,
    std::string name, bool arg );

void is_false( std::string file, unsigned int line,
    std::string name, bool arg );

void can_find( std::string file, unsigned int line,
    std::string lookinname, std::string lookforname,
    std::string lookin, std::string lookfor );

}

#endif

