
#include <iostream>

#include "lib/evaluationerror.h"
#include "test/assertionfailed.h"
#include "test/testarithmetic.h"
#include "test/testcomparators.h"
#include "test/testconstants.h"
#include "test/testdefineprocedure.h"
#include "test/testdefinesymbol.h"
#include "test/testenvironment.h"
#include "test/testif.h"
#include "test/testlambda.h"
#include "test/testmemory.h"
#include "test/testlexconstants.h"
#include "test/testparseconstants.h"
#include "test/testprettyprintconstants.h"
#include "test/testrepl.h"
#include "test/testtailcalloptimisation.h"
#include "test/testwordlexer.h"

using namespace std;

int main()
{
    try
    {
        TestWordLexer(            ).Run();
        TestLexConstants(         ).Run();
        TestParseConstants(       ).Run();
        TestPrettyPrintConstants( ).Run();
        TestMemory(               ).Run();
        TestConstants(            ).Run();
        TestRepl(                 ).Run();
        TestArithmetic(           ).Run();
        TestEnvironment(          ).Run();
        TestDefineSymbol(         ).Run();
        TestLambda(               ).Run();
        TestDefineProcedure(      ).Run();
        TestIf(                   ).Run();
        TestComparators(          ).Run();
        TestTailCallOptimisation( ).Run();
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

