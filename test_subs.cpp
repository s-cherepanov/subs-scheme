
#include <iostream>

#include "test/assertionfailed.h"
#include "test/testconstants.h"
#include "test/testparseconstants.h"

using namespace std;

int main()
{
    try
    {
        TestParseConstants().run();
        TestConstants().run();
    }
    catch( AssertionFailed& e )
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

