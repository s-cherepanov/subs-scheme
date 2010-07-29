
#ifndef SUBSINTERPRETER_H
#define SUBSINTERPRETER_H

#include <string>

#include "evaluator.h"

class SubsInterpreter
{
public:
    std::string Interpret( const std::string& codestring );
private:
    Evaluator evaluator_;
};

#endif

