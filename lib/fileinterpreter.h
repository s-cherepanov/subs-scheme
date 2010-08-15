
#ifndef FILEINTERPRETER_H
#define FILEINTERPRETER_H


#include <fstream>
#include <iostream>
#include <string>

#include "lib/subsinterpreter.h"

class FileInterpreter
{
public:
    FileInterpreter( const std::string& filename );

    int Interpret( std::ostream& outstream );

private:
    std::ifstream file_;
    SubsInterpreter interpreter_;
};

#endif


