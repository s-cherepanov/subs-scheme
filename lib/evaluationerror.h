
#ifndef EVALUATION_ERROR_H
#define EVALUATION_ERROR_H

#include <string>

class EvaluationError
{
public:
    EvaluationError( std::string msg );

    const std::string& ToString();

private:
    std::string msg_;
};

#endif

