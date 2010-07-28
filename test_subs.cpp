
#include <iostream>

#include "test/assertionfailed.h"
#include "test/testconstants.h"

using namespace std;

int main()
{
    try
    {
        TestConstants().run();
    }
    catch( AssertionFailed& e )
    {
        cerr << e.ToString() << endl;
        return 1;
    }

    return 0;
}

