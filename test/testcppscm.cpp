/*
 *  Subs Scheme Interpreter
 *  Copyright (C) 2010, 2011 Andy Balaam
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

#include <string>

#include "lib/cppscm/cppscm.h"
#include "test/assertmacros.h"
#include "test/cppscmassertmacros.h"
#include "test/testcppscm.h"

using namespace std;

namespace
{


void can_use_bools()
{
    ASSERT_CPPSCM_EQUAL( hashf, hashf );
    ASSERT_CPPSCM_NOT_EQUAL( hashf, hasht );
}


void lists_work()
{
    //ASSERT_CPPSCM_EQUAL( car( list, 1, 2, 3 ), I(1) );
}



}

#define SUITENAME "TestCppScm"

void TestCppScm::Run() const
{
    RUN_TEST(can_use_bools);
    // NOT IMPL RUN_TEST(can_use_ints);
    // FAILS RUN_TEST(lists_work);
}

