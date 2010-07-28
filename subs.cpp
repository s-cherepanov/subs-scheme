
#include <iostream>

#include "lib/subsrepl.h"

using namespace std;

int main()
{
    // TODO - a non-interactive mode
    return SubsRepl().Run( cin, cout, cerr );
}

