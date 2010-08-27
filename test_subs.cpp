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

#include <iostream>

#include "lib/evaluationerror.h"
#include "test/assertionfailed.h"
#include "test/testarithmetic.h"
#include "test/testbracketmatcher.h"
#include "test/testcomparators.h"
#include "test/testcond.h"
#include "test/testconstants.h"
#include "test/testdefineprocedure.h"
#include "test/testdefinesymbol.h"
#include "test/testenvironment.h"
#include "test/testif.h"
#include "test/testlambda.h"
#include "test/testlexer.h"
#include "test/testmemory.h"
#include "test/testnewlineprocessor.h"
#include "test/testparseconstants.h"
#include "test/testprettyprintconstants.h"
#include "test/testrepl.h"
#include "test/testtailcalloptimisation.h"
#include "test/testtokenlist.h"

using namespace std;

int main()
{
    try
    {
        TestLexer(                ).Run();
        TestParseConstants(       ).Run();
        TestPrettyPrintConstants( ).Run();
        TestMemory(               ).Run();
        TestConstants(            ).Run();
        TestTokenList(            ).Run();
        TestNewLineProcessor(     ).Run();
        TestBracketMatcher(       ).Run();
        TestRepl(                 ).Run();
        TestArithmetic(           ).Run();
        TestEnvironment(          ).Run();
        TestDefineSymbol(         ).Run();
        TestLambda(               ).Run();
        TestDefineProcedure(      ).Run();
        TestIf(                   ).Run();
        TestComparators(          ).Run();
        TestTailCallOptimisation( ).Run();
        TestCond(                 ).Run();
    }
    catch( AssertionFailed& e )
    {
        cerr << e.ToString() << endl;
        cout << "Some tests failed." << endl;
        return 1;
    }
    catch( EvaluationError& e )
    {
        cerr << e.ToString() << endl;
        cout << "Some tests failed." << endl;
        return 1;
    }
    catch( exception& e )
    {
        cerr << e.what() << endl;
        cout << "Some tests failed." << endl;
        return 1;
    }

    cout << "All tests passed." << endl;
    return 0;
}

