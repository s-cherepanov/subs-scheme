
#ifndef SUBSINTERPRETER_H
#define SUBSINTERPRETER_H

#include <iostream>
#include <memory>
#include <string>

#include "evaluator.h"

class Tracer;

class SubsInterpreter
{
public:
    std::string Interpret( const std::string& codestring );

    int InterpretStream( std::istream& instream, std::ostream& outstream );

    void SetTracer( Tracer* tracer );

private:
    Evaluator evaluator_;
};

#endif

