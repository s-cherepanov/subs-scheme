
#include <string>

#include "evaluationerror.h"

using namespace std;

EvaluationError::EvaluationError( std::string msg )
: msg_( msg )
{
}

const std::string& EvaluationError::ToString()
{
    return msg_;
}

