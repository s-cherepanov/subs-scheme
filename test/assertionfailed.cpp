
#include <iostream> // todo remove

#include <sstream>
#include <string>

#include "assertionfailed.h"

using namespace std;

string AssertionFailed::ToString()
{
    ostringstream ss;
    ss << file_ << ":" << line_ << " - Assertion failed: " << msg_;
    return ss.str();
}


